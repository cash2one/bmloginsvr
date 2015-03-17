package main

import (
	"encoding/json"
	"log"
	"redigo/redis"
	"time"
)

//	输出的Redis信息
const (
	RedisEvent_Undefine = iota
	RedisEvent_SavePlayerData
)

type RedisEvent struct {
	CommandType int   `json:"commandType"`
	Arguments   []int `json:"arguments"`
	BinaryData  []byte
}

type RedisOperator struct {
	outputChan chan *RedisEvent //	输出队列 供main监听
	inputChan  chan *RedisEvent // 输入队列 用于go发送命令给redis
	queueName  string           //	队列名称
	r          redis.Conn
}

func NewRedisOperator() *RedisOperator {
	ret := &RedisOperator{}
	ret.outputChan = make(chan *RedisEvent, 100)
	ret.inputChan = make(chan *RedisEvent, 100)
	return ret
}

func (this *RedisOperator) ExecuteCommand(evt *RedisEvent) {
	this.inputChan <- evt
}

func (this *RedisOperator) Run(addr string, queueName string) bool {
	var err error
	this.r, err = redis.Dial("tcp", addr)

	if err != nil {
		log.Println("redis error : ", this.r.Err())
		return false
	}
	if this.r.Err() != nil {
		log.Println("redis error : ", this.r.Err())
		return false
	}

	this.queueName = queueName

	go this.go_popRedis()
	return true
}

func (this *RedisOperator) go_popRedis() {
	log.Println("Goroutine [go_popRedis] start...")
	defer log.Println("Goroutine [go_popRedis] quit...")

	for {
		select {
		case <-time.After(time.Millisecond * 500):
			{
				//	pop all events
				for {
					buf, err := redis.Bytes(this.r.Do("LPOP", this.queueName))
					if err != nil ||
						len(buf) == 0 {
						break
					}

					log.Println(string(buf))

					//	get the event
					evt := &RedisEvent{}
					err = json.Unmarshal(buf, evt)
					if err != nil {
						log.Println("json unmarshal error:", err)
						continue
					}

					log.Println("redis event [", evt.CommandType, "] received")

					//	get ok, write to output channel
					//this.outputChan <- evt
					//	can internal proceed?
					switch evt.CommandType {
					case RedisEvent_SavePlayerData:
						{
							//	get the playerdata hash map
							this.doSavePlayerData(evt)
						}
					}
				}
			}
		}
	}

	this.r.Close()
	this.r = nil
}

func (this *RedisOperator) doSavePlayerData(evt *RedisEvent) {
	ret, err := this.r.Do("EXISTS", "playerdata")
	if nil != err {
		log.Println("redis error:", err)
		return
	}

	if 0 == ret {
		//	error
		return
	} else {
		//	get the hash data
		ret, err = this.r.Do("HKEYS", "playerdata")
		if err != nil {
			log.Println("redis error:", err)
			return
		}

		if nil == ret {
			return
		}

		keySlice := ret.([]interface{})
		for _, v := range keySlice {
			key := string(v.([]uint8))

			//	get the playerdata key, now get the data
			var value interface{}
			value, err = this.r.Do("HGET", "playerdata", key)
			if nil != err {
				log.Println("redis error:", err)
				continue
			} else {
				data := value.([]uint8)
				log.Println("saving player [", key, "] hum data...")
				chanEvt := &RedisEvent{}
				chanEvt.CommandType = RedisEvent_SavePlayerData
				chanEvt.BinaryData = data
				this.outputChan <- chanEvt
			}

			//	delete the key
			_, err = this.r.Do("HDEL", "playerdata", key)
			if err != nil {
				log.Println("redis error:", err)
			}
		}
	}
}
