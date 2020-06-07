// ETH32nix - an Enemy Territory cheat for Linux
// Copyright (c) 2008 eth32 team
// www.cheatersutopia.com & www.nixcoders.org

#include "eth32.h"
#include "puresounds.h"

CSounds Sounds;

// when replaced sounds are used, a lookup table is generated to quickly switch sounds on the fly.
// the index is a regular sfxHandle, and it points to a eth32 sound struct

int csnd_hash[8192];

void CSounds::Init()
{
	gameSounds = NULL;
	nGameSounds = nIgnore = 0;
	memset(csnd_hash, 0, sizeof(csnd_hash));
}

void CSounds::ShutDown()
{
	if (gameSounds) free(gameSounds);
	nGameSounds = nIgnore = 0;
}

/*void CSounds::Respawn()
{
	spree = -2;
}*/

// all legit sounds pass trough here, therefore,
// sounds that dont exist csnd_hash are eth32 sounds
int CSounds::HookSounds(char *name, int compressed)
{
	if (!name)
		return 0;

	gameSounds = (eth32Sounds_t *)realloc(gameSounds, (nGameSounds+1)*sizeof(eth32Sounds_t));
	strncpy(gameSounds[nGameSounds].name, name, 255);

	sfxHandle_t snd = Syscall.S_RegisterSound(name, (qboolean)compressed);
	gameSounds[nGameSounds].orig = snd;

	gameSounds[nGameSounds].replaced = false;

	int j = 0;
	gameSounds[nGameSounds].pure = false;
	while (pureSounds[j]) {
		if (!strcmp(pureSounds[j], name)){
			gameSounds[nGameSounds].pure = true;
			break;
		}
		j++;
	}

	gameSounds[nGameSounds].hqsound = !strncmp(name, "hq_", 3) ? true : false;

	csnd_hash[(int)snd] = nGameSounds;
	nGameSounds++;
	return snd;
}

// test wether a sound has a replacement :)
/*bool CSounds::existReplacement(char *sound)
{
	char csnd[256];
	char *base = (char*)strrchr(sound, '/');
	char *snd = base+1;
	csnd[0] = '\0';

	if (base) {
		strncat(csnd, sound, base-sound);
		strcat(csnd, "/eth32_");
		strcat(csnd, snd);
	} else
		sprintf(csnd, "eth32_%s", sound);

	fileHandle_t f;
	int len = Syscall.FS_FOpenFile(csnd, &f, FS_READ);

	if (!f)
		return false;

	Syscall.FS_FCloseFile(f);
	return true;
}*/

bool CSounds::Process(int type, int *args)
{
	if (!gameSounds)
		return true;

	int sndHnd = (type == CG_S_STARTLOCALSOUND) ? args[0] : args[3];
	eth32Sounds_t *snd = &gameSounds[csnd_hash[sndHnd]];

	//if (!eth32.settings.hqSounds && snd->hqsound)
	//	return false;

	if (eth32.settings.pureSounds && !snd->pure)
		return false;

	return true;
}

// spree is actually the multikill amount
// a multikill sound gets played if spree works its way up to a value equal to or above 0
// meaning double kill or more
/*void CSounds::ProcessMultikillSounds( void )
{
	Syscall.S_FadeAllSound(-1, 0, qtrue); // stop sound
	Syscall.S_FadeAllSound(1.0f, 0, qfalse); // fade sound back up after stop above

	spree++;

	if( (eth32.cg.time - soundDelay) > eth32.settings.mkResetTime ){
		spree = 0;
		soundDelay = eth32.cg.time;
		return;
	}

	if (spree >= 0) {
		if(spree >= SOUND_LEVEL_MAX)
			spree = SOUND_LEVEL_MAX - 1;

		randNum = (rand() % (SOUND_LEVEL_SIZE_MAX));
		Syscall.S_StartLocalSound(eth32.cg.media.sounds.playsound[spree][randNum], CHAN_LOCAL_SOUND);
	}
}*/

/*// still not the best but working :>
const char *defaultSounds[25] =
{
	"sounds/wowMan.wav",
	"sounds/haha.wav",
	"sounds/excellent.wav",
	"sounds/impressive.wav",
	"sounds/boomheadshot.wav",
	"sounds/multikill2.wav",
	"sounds/dominating1.wav",
	"sounds/ultrakill2.wav",
	"sounds/unstoppable2.wav",
	"sounds/godlike1.wav",
	"sounds/hattrick.wav",
	"sounds/multikillw.wav",
	"sounds/multikill.wav",
	"sounds/wickedsick.wav",
	"sounds/triple.wav",
	"sounds/rampage.wav",
	"sounds/triple2.wav",
	"sounds/ultrakill.wav",
	"sounds/unstoppable.wav",
	"sounds/monsterkill.wav",
	"sounds/monsterkillw.wav",
	"sounds/perfect.wav",
	"sounds/unreal.wav",
	"sounds/ludic.wav",
	"sounds/holy.wav",
};*/

void CSounds::RegisterSounds( void )
{
	//sprintf(settingsFile, "%s/%s", eth32.path, ETH32_SETTINGS);

	//Hitsounds
	/*eth32.cg.media.sounds.hitsound = Syscall.S_RegisterSound("sounds/hitsound/head.wav", qtrue);
	eth32.cg.media.sounds.hitsound2 = Syscall.S_RegisterSound("sounds/hitsound/headshot.wav", qtrue);
	eth32.cg.media.sounds.hitsound3 = Syscall.S_RegisterSound("sounds/hitsound/body.wav", qtrue);
	eth32.cg.media.sounds.hitsoundtm = Syscall.S_RegisterSound("sounds/hitsound/team.wav", qtrue);*/
	eth32.cg.media.sounds.hitsounds[HIT_HEAD] = Syscall.S_RegisterSound("sounds/hitsound/head.wav", qtrue);
	eth32.cg.media.sounds.hitsounds[HIT_HEADSHOT] = Syscall.S_RegisterSound("sounds/hitsound/headshot.wav", qtrue);
	eth32.cg.media.sounds.hitsounds[HIT_BODY] = Syscall.S_RegisterSound("sounds/hitsound/body.wav", qtrue);
	eth32.cg.media.sounds.hitsounds[HIT_TEAM] = Syscall.S_RegisterSound("sounds/hitsound/team.wav", qtrue);

	//Spree sounds
	// level 1
	eth32.cg.media.sounds.spree[SOUND_DOUBLEKILL1] = Syscall.S_RegisterSound("sounds/spree/level1/doublekill1.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_EXCELLENT] = Syscall.S_RegisterSound("sounds/spree/level1/excellent.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_IMPRESSIVE] = Syscall.S_RegisterSound("sounds/spree/level1/boomheadshot.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_KILLINGSPREE1] = Syscall.S_RegisterSound("sounds/spree/level1/killingspree1.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_KILLINGSPREE2] = Syscall.S_RegisterSound("sounds/spree/level1/combowhore.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_PERFECT] = Syscall.S_RegisterSound("sounds/spree/level1/hattrick.wav", qtrue);
	// level 2
	eth32.cg.media.sounds.spree[SOUND_GODLIKE1] = Syscall.S_RegisterSound("sounds/spree/level2/godlike1.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_GODLIKE2] = Syscall.S_RegisterSound("sounds/spree/level2/godlike2.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_MULTIKILL1] = Syscall.S_RegisterSound("sounds/spree/level2/multikill1.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_MULTIKILL2] = Syscall.S_RegisterSound("sounds/spree/level2/rampage.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_MULTIKILL3] = Syscall.S_RegisterSound("sounds/spree/level2/multikill3.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_TRIPLEKILL] = Syscall.S_RegisterSound("sounds/spree/level2/triplekill.wav", qtrue);
	// level 3
	eth32.cg.media.sounds.spree[SOUND_DOMINATING1] = Syscall.S_RegisterSound("sounds/spree/level3/dominating1.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_ULTRAKILL1] = Syscall.S_RegisterSound("sounds/spree/level3/ultrakill1.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_ULTRAKILL2] = Syscall.S_RegisterSound("sounds/spree/level3/unreal.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_ULTRAKILL3] = Syscall.S_RegisterSound("sounds/spree/level3/ultrakill3.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_UNREAL] = Syscall.S_RegisterSound("sounds/spree/level3/ludic.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_MEGAKILL] = Syscall.S_RegisterSound("sounds/spree/level3/megakill.wav", qtrue);
	// level 4
	eth32.cg.media.sounds.spree[SOUND_MONSTERKILL1] = Syscall.S_RegisterSound("sounds/spree/level4/monsterkill1.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_MONSTERKILL2] = Syscall.S_RegisterSound("sounds/spree/level4/monsterkill2.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_HOLYSHIT] = Syscall.S_RegisterSound("sounds/spree/level4/holyshit.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_UNSTOPPABLE1] = Syscall.S_RegisterSound("sounds/spree/level4/unstoppable1.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_UNSTOPPABLE2] = Syscall.S_RegisterSound("sounds/spree/level4/unstoppable2.wav", qtrue);
	eth32.cg.media.sounds.spree[SOUND_WICKEDSICK] = Syscall.S_RegisterSound("sounds/spree/level4/wickedsick.wav", qtrue);
	
	/*char value[1024];
	char buf[64];
	int soundName = 0;

	for(int t = 0; t < SOUND_LEVEL_MAX; t++){
		for(int s = 0; s < SOUND_LEVEL_SIZE_MAX; s++) {
			sprintf(buf, "sound%i%i", t, s);
			Tools.GetPrivateProfileString("Sound Settings", buf, (char*)defaultSounds[soundName], value, sizeof(value), settingsFile);

			eth32.cg.media.sounds.playsound[t][s] = Syscall.S_RegisterSound(value, qtrue);
			soundName++;
		}

	}

	this->spree = 0;*/

#ifdef ETH32_DEBUG
	Debug.Log("All Game Sounds Registered...");
#endif
}
