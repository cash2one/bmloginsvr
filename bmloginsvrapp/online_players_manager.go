package main

type OnlinePlayerManager struct {
	onlinePlayers map[uint32]uint32
}

func NewOnlinePlayerManager() *OnlinePlayerManager {
	instance := &OnlinePlayerManager{}
	instance.onlinePlayers = make(map[uint32]uint32)
	return instance
}

func (this *OnlinePlayerManager) GetPlayerConnectedServerTag(uid uint32) uint32 {
	tag, ok := this.onlinePlayers[uid]
	if !ok {
		return 0
	}
	return tag
}

func (this *OnlinePlayerManager) SetPlayerConnectedServerTag(uid uint32, tag uint32) {
	this.onlinePlayers[uid] = tag
}

func (this *OnlinePlayerManager) RemovePlayerConnectedServerTag(uid uint32) {
	delete(this.onlinePlayers, uid)
}
