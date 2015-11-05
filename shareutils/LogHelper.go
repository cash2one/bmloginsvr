package shareutils

import (
	"errors"
	"fmt"
	"log"
	"os"
	"runtime"
	"strconv"
	"strings"
	"time"
)

/*
	setting example:
	-tags="LogToFile:false_LogPrefix:LEVEL+DATE+TIME+FILELINE"
*/

const (
	//name_enablefilelogging = "enableFileLogging"
	//	2	settings
	//cfg_settingscounter = 2
	cfg_logtofile_0      = "LogToFile"      //	"LogToFile:true|false"
	cfg_loglasttime_1    = "LogLastTime"    //	"LogLastTime:1|2|..."
	cfg_logpriority_2    = "LogPriority"    //	"LogPriority:DEBUG|INFO|..."
	cfg_logprefix_3      = "LogPrefix"      //	"LogPrefix:LEVEL+DAY+TIME+FILELINE|DEFAULT"
	cfg_loglevelprefix_4 = "LogLevelPrefix" //	"LogLevelPrefix:0|1"	(0:不显示 1:显示)
)

const (
	usg_settingscounter = 2
	usg_logtofile_0     = "LogToFile:true|false"
	usg_loglasttime_1   = "LogLastTime:1|2|..."
	usg_logpriority_2   = "LogPriority:DEBUG|INFO|..."
	usg_logprefix_3     = "LogPrefix:LEVEL+DAY+TIME+FILELINE|DEFAULT"
)

const (
	LOGL_DEBUG = iota
	LOGL_INFO
	LOGL_WARN
	LOGL_ERROR
	LOGL_FATAL
)

const (
	LOGP_LEVEL = 1 << iota
	LOGP_DAY
	LOGP_TIME
	LOGP_FILELINE

	//LOGP_DEFAULT = LOGP_LEVEL | LOGP_DAY | LOGP_TIME
	LOGP_DEFAULT = LOGP_LEVEL
)

type LogHelper struct {
	file            *os.File
	logtofile       bool
	loglasttime     int
	logcreatetime   time.Time
	logpriority     int
	logprefix       int
	loglevelprefix  int
	logexpiretime   uint32
	logbasefilename string
	initialized     bool
	stackLevel      int
}

var (
	g_LogHelper LogHelper
)

///////////////////////////////////////////////////////////////////////////
func DefaultLogHelper() *LogHelper {
	return &g_LogHelper
}

///////////////////////////////////////////////////////////////////////////
func (this *LogHelper) Init(logfilename string, setting string) error {
	this.logtofile = true
	this.logcreatetime = time.Now()
	this.loglasttime = 1
	this.logpriority = LOGL_DEBUG
	//log.SetFlags(0)
	this.initialized = true
	this.logprefix = LOGP_DEFAULT
	this.stackLevel = 0

	this.Release()
	var err error

	//	Get all settings
	settings := strings.Split(setting, "_")
	for _, v := range settings {
		if len(v) == 0 {
			continue
		}

		cfgidx, sel := searchCfgIndex(v)
		if cfgidx != -1 {
			this.updateSettings(cfgidx, sel)
		} else {
			return errors.New("Unsupport config section[" + v + "]")
		}
	}

	logoutputfile := logfilename
	if len(logoutputfile) > 0 {
		this.logbasefilename = logfilename
		logoutputfile = getLogFileName(logfilename)
	} else {
		//return errors.New("No input file specified")
		log.Println("No input file specified.Using default file[default]")
		logoutputfile = getLogFileName("default")
	}

	if this.logtofile {
		this.file, err = os.Create(logoutputfile)
		if err != nil {
			this.logtofile = false
			return err
		}
		log.SetOutput(this.file)

		//	calculate expire time
	}

	log.Printf("Current log system settings: LogToFile[%s] | LogLastTime[%d] | LogOutputFile[%s] | LogPriority[%s] | LogPrefix[%s]",
		boolToString(this.logtofile), this.loglasttime, logoutputfile, priorityToString(this.logpriority), prefixToString(this.logprefix))

	//	change flags
	this.applyFlags()

	return nil
}

func (this *LogHelper) applyFlags() {
	flags := 0

	if (this.logprefix & LOGP_LEVEL) != 0 {
		//	nothing
	}
	if (this.logprefix & LOGP_DAY) != 0 {
		flags |= log.Ldate
	}
	if (this.logprefix & LOGP_TIME) != 0 {
		flags |= log.Ltime
	}
	if (this.logprefix & LOGP_FILELINE) != 0 {
		flags |= log.Lshortfile
	}

	//log.SetFlags(0)
}

func (this *LogHelper) getLogLevelPrefix(level int) string {
	if 0 == this.loglevelprefix {
		return ""
	}

	prefix := "[U]"

	switch level {
	case LOGL_DEBUG:
		{
			prefix = "[D]"
		}
	case LOGL_WARN:
		{
			prefix = "[W]"
		}
	case LOGL_ERROR:
		{
			prefix = "[E]"
		}
	case LOGL_INFO:
		{
			prefix = "[I]"
		}
	case LOGL_FATAL:
		{
			prefix = "[F]"
		}
	}

	return prefix
}

func (this *LogHelper) Release() {
	if nil != this.file {
		this.file.Close()
		this.file = nil
	}
}

func (this *LogHelper) GetFlag() int {
	return this.logprefix
}

func (this *LogHelper) SetFlag(flag int) {
	this.logprefix = flag
}

func (this *LogHelper) Debugln(v ...interface{}) {
	this.logln(LOGL_DEBUG, v...)
}

func (this *LogHelper) Infoln(v ...interface{}) {
	this.logln(LOGL_INFO, v...)
}

func (this *LogHelper) Warnln(v ...interface{}) {
	this.logln(LOGL_WARN, v...)
}

func (this *LogHelper) Errorln(v ...interface{}) {
	this.logln(LOGL_ERROR, v...)
}

func (this *LogHelper) Fatalln(v ...interface{}) {
	this.logln(LOGL_FATAL, v...)
}

func (this *LogHelper) Debugf(format string, v ...interface{}) {
	this.logf(LOGL_DEBUG, format, v...)
}

func (this *LogHelper) Infof(format string, v ...interface{}) {
	this.logf(LOGL_INFO, format, v...)
}

func (this *LogHelper) Warnf(format string, v ...interface{}) {
	this.logf(LOGL_WARN, format, v...)
}

func (this *LogHelper) Errorf(format string, v ...interface{}) {
	this.logf(LOGL_ERROR, format, v...)
}

func (this *LogHelper) Fatalf(format string, v ...interface{}) {
	this.logf(LOGL_FATAL, format, v...)
}

//////////////////////////////////////////////////////////////////////////
//	private
func (this *LogHelper) logf(level int, format string, v ...interface{}) {
	if !this.initialized {
		log.Printf(format, v...)
		return
	}

	this.checkDate()

	if level < this.logpriority {
		return
	}

	prefix := this.getPrefix(level)
	if len(prefix) != 0 {
		content := fmt.Sprintf(format, v...)
		log.Println(prefix, " ", content)
	} else {
		log.Printf(format, v...)
	}
	/*prefix := this.getLogLevelPrefix(level)
	log.SetPrefix(prefix)
	log.Printf(format, v...)
	log.SetPrefix("")*/

	if level == LOGL_FATAL {
		panic("Fatal error, terminated.")
		os.Exit(1)
	}
}

func (this *LogHelper) logln(level int, v ...interface{}) {
	if !this.initialized {
		log.Println(v...)
		return
	}

	this.checkDate()

	if level < this.logpriority {
		return
	}

	prefix := this.getPrefix(level)
	if len(prefix) != 0 {
		content := fmt.Sprintln(v...)
		log.Print(prefix, " ", content)
	} else {
		log.Println(v...)
	}
	/*prefix := this.getLogLevelPrefix(level)
	log.SetPrefix(prefix)
	log.Println(v...)
	log.SetPrefix("")*/

	if level == LOGL_FATAL {
		panic("Fatal error, terminated.")
		os.Exit(1)
	}
}

func (this *LogHelper) getPrefix(level int) string {
	//now := time.Now()
	if 0 == this.logprefix {
		return ""
	}

	var prefix string = "[#"

	if (this.logprefix & LOGP_LEVEL) != 0 {
		prefix += priorityToSString(level)
	}
	/*if (this.logprefix & LOGP_DAY) != 0 {
		if len(prefix) == 0 || (len(prefix) == 1 && prefix == "[") {
			prefix += GetDateString(now, "/")
		} else {
			prefix += " " + GetDateString(now, "/")
		}
	}
	if (this.logprefix & LOGP_TIME) != 0 {
		if len(prefix) == 0 || (len(prefix) == 1 && prefix == "[") {
			prefix += GetTimeString(now, ":")
		} else {
			prefix += " " + GetTimeString(now, ":")
		}
	}*/
	if (this.logprefix & LOGP_FILELINE) != 0 {
		stackLevel := this.stackLevel
		if stackLevel == 0 {
			stackLevel = 3
		}
		_, file, line, ok := runtime.Caller(stackLevel)
		if !ok {
			file = "???"
			line = 0
		}

		if len(file) > 0 {
			subfile := strings.Split(file, "/")
			if len(subfile) > 0 {
				file = subfile[len(subfile)-1]
			}
		}

		if len(prefix) == 0 || (len(prefix) == 1 && prefix == "[") {
			prefix += file + ":" + strconv.Itoa(line)
		} else {
			prefix += " " + file + ":" + strconv.Itoa(line)
		}
	}

	if prefix == "[#" {
		return ""
	} else {
		prefix += "]"
	}

	return prefix
}

func (this *LogHelper) updateSettings(cfgidx int, sel string) {
	sel = strings.ToLower(sel)

	switch cfgidx {
	case 0:
		{
			if "true" == sel {
				this.logtofile = true
			} else {
				this.logtofile = false
			}
		}
	case 1:
		{
			loglasttime, err := strconv.Atoi(sel)
			if err == nil {
				this.loglasttime = loglasttime
			}
			if this.loglasttime == 0 {
				this.loglasttime = 1
			}
		}
	case 2:
		{
			if "debug" == sel {
				this.logpriority = LOGL_DEBUG
			} else if "info" == sel {
				this.logpriority = LOGL_INFO
			} else if "warn" == sel {
				this.logpriority = LOGL_WARN
			} else if "error" == sel {
				this.logpriority = LOGL_ERROR
			} else if "fatal" == sel {
				this.logpriority = LOGL_FATAL
			}
		}
	case 3:
		{
			prefixes := strings.Split(sel, "+")
			var defprefix int = 0

			if len(prefixes) > 0 {
				for _, prefix := range prefixes {

					if "level" == prefix {
						defprefix |= LOGP_LEVEL
					} else if "date" == prefix {
						//defprefix |= LOGP_DAY
					} else if "time" == prefix {
						//defprefix |= LOGP_TIME
					} else if "fileline" == prefix {
						defprefix |= LOGP_FILELINE
					} else if "default" == prefix {
						//defprefix |= LOGP_DEFAULT
					}
				}
			}

			if defprefix != 0 {
				this.SetFlag(defprefix)
			}
		}
	case 4:
		{
			levelprefix, err := strconv.Atoi(sel)
			if err == nil {
				this.loglevelprefix = levelprefix
			}
		}
	}
}

func searchCfgIndex(cfg string) (int, string) {
	cfgstr := []string{
		cfg_logtofile_0,
		cfg_loglasttime_1,
		cfg_logpriority_2,
		cfg_logprefix_3,
		cfg_loglevelprefix_4,
	}

	for i, v := range cfgstr {
		v = strings.ToLower(v)
		cfg = strings.ToLower(cfg)

		if strings.Contains(cfg, v) {
			retstr := strings.Split(cfg, ":")
			if len(retstr) == 2 {
				return i, retstr[1]
			}
		}
	}

	return -1, ""
}

func (this *LogHelper) checkDate() {
	if this.logtofile && this.file != nil {

	}
}

func boolToString(b bool) string {
	if b {
		return "true"
	}
	return "false"
}

func priorityToString(p int) string {
	if p == LOGL_DEBUG {
		return "DEBUG"
	} else if p == LOGL_INFO {
		return "INFO"
	} else if p == LOGL_WARN {
		return "WARN"
	} else if p == LOGL_ERROR {
		return "ERROR"
	} else if p == LOGL_FATAL {
		return "FATAL"
	}
	return "UNKNOWN"
}

func priorityToSString(p int) string {
	if p == LOGL_DEBUG {
		return "D"
	} else if p == LOGL_INFO {
		return "I"
	} else if p == LOGL_WARN {
		return "W"
	} else if p == LOGL_ERROR {
		return "E"
	} else if p == LOGL_FATAL {
		return "F"
	}
	return "U"
}

func prefixToString(p int) string {
	var prefixes string

	if 0 == p {
		return "NONE"
	}

	if (p & LOGP_LEVEL) != 0 {
		prefixes += "LEVEL"
	}
	if (p & LOGP_DAY) != 0 {
		if len(prefixes) != 0 {
			prefixes += "+"
		}
		prefixes += "DATE"
	}
	if (p & LOGP_TIME) != 0 {
		if len(prefixes) != 0 {
			prefixes += "+"
		}
		prefixes += "TIME"
	}
	if (p & LOGP_FILELINE) != 0 {
		if len(prefixes) != 0 {
			prefixes += "+"
		}
		prefixes += "FILELINE"
	}

	if len(prefixes) != 0 {
		return prefixes
	}
	return "UNKNOWN"
}

func getLogFileName(basename string) string {
	basename += "-"
	basename += GetDateString(time.Now(), ".") + ".log"
	return basename
}

func GetTimeString(tm time.Time, spliter string) string {
	var timestr string

	if tm.Hour() >= 10 {
		timestr += strconv.Itoa(tm.Hour())
	} else {
		timestr += "0" + strconv.Itoa(tm.Hour())
	}
	timestr += spliter

	if tm.Minute() >= 10 {
		timestr += strconv.Itoa(tm.Minute())
	} else {
		timestr += "0" + strconv.Itoa(tm.Minute())
	}
	timestr += spliter

	if tm.Second() >= 10 {
		timestr += strconv.Itoa(tm.Second())
	} else {
		timestr += "0" + strconv.Itoa(tm.Second())
	}
	return timestr
}

func GetDateString(tm time.Time, spliter string) string {
	var datestr string
	year, month, day := tm.Date()

	datestr += strconv.Itoa(year)
	datestr += spliter

	if int(month) >= 10 {
		datestr += strconv.Itoa(int(month))
	} else {
		datestr += "0" + strconv.Itoa(int(month))
	}
	datestr += spliter

	if day >= 10 {
		datestr += strconv.Itoa(day)
	} else {
		datestr += "0" + strconv.Itoa(day)
	}

	return datestr
}

///////////////////////////////////////////////////////////////////////////
func (this *LogHelper) LoadLogConfig(cfgfile string, logfile string) error {
	return nil
}

////////////////////////////////////////////////////////////////////////////
//	pure function call
func setStackLevel(_level int) {
	g_LogHelper.stackLevel = _level
}

func LogDebugln(v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Debugln(v...)
	setStackLevel(0)
}

func LogDebugf(format string, v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Debugf(format, v...)
	setStackLevel(0)
}

func LogInfoln(v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Infoln(v...)
	setStackLevel(0)
}

func LogInfof(format string, v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Infof(format, v...)
	setStackLevel(0)
}

func LogWarnln(v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Warnln(v...)
	setStackLevel(0)
}

func LogWarnf(format string, v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Warnf(format, v...)
	setStackLevel(0)
}

func LogErrorln(v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Errorln(v...)
	setStackLevel(0)
}

func LogErrorf(format string, v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Errorf(format, v...)
	setStackLevel(0)
}

func LogFatalln(v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Fatalln(v...)
	setStackLevel(0)
}

func LogFatalf(format string, v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Fatalf(format, v...)
	setStackLevel(0)
}

func LogPrintln(v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Infoln(v...)
	setStackLevel(0)
}

func LogPrintf(format string, v ...interface{}) {
	setStackLevel(4)
	g_LogHelper.Infof(format, v...)
	setStackLevel(0)
}

func LogInit(filename string, settings string) error {
	return g_LogHelper.Init(filename, settings)
}
