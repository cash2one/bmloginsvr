package eventdispatcher

import (
	"container/list"
)

type CallbackFunc func(interface{})

type listener struct {
	callback CallbackFunc
	id       int
}

type eventListenerList struct {
	listeners *list.List
}

func (this *eventListenerList) call(data interface{}) int {
	count := 0
	for fr := this.listeners.Front(); fr != nil; fr = fr.Next() {
		lis, ok := fr.Value.(listener)
		if ok {
			lis.callback(data)
			count++
		}
	}

	return count
}

func (this *eventListenerList) add(id int, cb CallbackFunc) {
	var lis listener
	lis.id = id
	lis.callback = cb
	this.listeners.PushBack(lis)
}

func (this *eventListenerList) remove(id int) bool {
	for fr := this.listeners.Front(); fr != nil; fr = fr.Next() {
		lis, ok := fr.Value.(listener)
		if ok {
			if lis.id == id {
				this.listeners.Remove(fr)
				return true
			}
		}
	}

	return false
}

func (this *eventListenerList) clear() {
	this.listeners.Init()
}

func (this *eventListenerList) count() int {
	return this.listeners.Len()
}

func newEventListenerList() *eventListenerList {
	ins := &eventListenerList{}
	ins.listeners = list.New()
	return ins
}

type EventDispatcher struct {
	listenerSeq    int
	listenerChains map[int]*eventListenerList
}

func (this *EventDispatcher) AddListener(eventId int, cb CallbackFunc) int {
	listeners, ok := this.listenerChains[eventId]
	if !ok {
		listeners = newEventListenerList()
		this.listenerChains[eventId] = listeners
	}

	this.listenerSeq++
	listeners.add(this.listenerSeq, cb)
	return this.listenerSeq
}

func (this *EventDispatcher) RemoveListener(id int) bool {
	for _, v := range this.listenerChains {
		if v.remove(id) {
			return true
		}
	}

	return false
}

func (this *EventDispatcher) Dispatch(eventId int, data interface{}) {
	listeners, ok := this.listenerChains[eventId]
	if !ok {
		return
	}

	listeners.call(data)
}

func NewEventDispatcher() *EventDispatcher {
	ins := &EventDispatcher{}
	ins.listenerChains = make(map[int]*eventListenerList)
	return ins
}
