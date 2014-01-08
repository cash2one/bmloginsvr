package main

type UserInfoList struct {
	allusers map[uint32]IUserInterface
}

func (this *UserInfoList) AddUser(user IUserInterface) bool {
	inlistuser := this.allusers[user.GetUserTag()]
	if nil == inlistuser {
		this.allusers[user.GetUserTag()] = user
		return true
	}
	return false
}

func (this *UserInfoList) GetUser(conntag uint32) IUserInterface {
	user := this.allusers[conntag]
	return user
}

func (this *UserInfoList) RemoveUser(conntag uint32) {
	delete(this.allusers, conntag)
}
