package main

import (
	"container/list"
	"github.com/jakecoffman/cron"
	"shareutils"
	"strconv"
)

type SchduleActiveCallback func(int)

type ScheduleJob struct {
	id       int
	callback SchduleActiveCallback
	data     interface{}
}

func NewScheduleJob(_id int, _job SchduleActiveCallback) *ScheduleJob {
	instance := &ScheduleJob{
		id:       _id,
		callback: _job,
	}
	return instance
}

func (this *ScheduleJob) Run() {
	if nil != this.callback {
		this.callback(this.id)
	} else {
		shareutils.LogErrorln("Invalid callback function")
	}
}

type ScheduleManager struct {
	cronJob *cron.Cron
	jobs    *list.List
}

func NewScheduleManager() *ScheduleManager {
	instance := &ScheduleManager{}
	instance.cronJob = cron.New()
	instance.jobs = list.New()
	return instance
}

func (this *ScheduleManager) Start() {
	this.cronJob.Start()
}

func (this *ScheduleManager) Stop() {
	this.cronJob.Stop()
}

func (this *ScheduleManager) GetJobs() *list.List {
	return this.jobs
}

func (this *ScheduleManager) AddJob(id int, scheduleExpr string) *ScheduleJob {
	//	test if same id exists
	for e := this.jobs.Front(); e != nil; e = e.Next() {
		j := e.Value.(*ScheduleJob)
		if j.id == id {
			shareutils.LogErrorln("Job id:", id, "already exists")
			return nil
		}
	}

	job := NewScheduleJob(id, this._scheduleActive)
	this.jobs.PushBack(job)
	this.cronJob.AddJob(scheduleExpr, job, strconv.Itoa(id))
	return job
}

func (this *ScheduleManager) RemoveJob(id int) {
	for e := this.jobs.Front(); e != nil; e = e.Next() {
		j := e.Value.(*ScheduleJob)
		if j.id == id {
			this.jobs.Remove(e)
			break
		}
	}
	this.cronJob.RemoveJob(strconv.Itoa(id))
}

func (this *ScheduleManager) GetJob(id int) *ScheduleJob {
	for e := this.jobs.Front(); e != nil; e = e.Next() {
		j := e.Value.(*ScheduleJob)
		if j.id == id {
			return j
		}
	}
	return nil
}

func (this *ScheduleManager) _scheduleActive(id int) {
	msg := &MThreadMsg{}
	msg.Event = kMThreadMsg_ScheduleActive
	msg.WParam = id
	PostMThreadMsg(msg)
}

var g_scheduleManager *ScheduleManager

func init() {
	g_scheduleManager = NewScheduleManager()
}
