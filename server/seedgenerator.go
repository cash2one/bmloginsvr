package server

var (
	gSeed    uint32 = 0
	seedonce bool   = false
)

func InitSeed(beg uint32) {
	if !seedonce {
		seedonce = true
	} else {
		return
	}
	gSeed = beg
}

func GetSeed() uint32 {
	gSeed++
	return gSeed
}
