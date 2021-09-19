#pragma once

#ifndef _GAME_H_
#define _GAME_H_

#include "stdafx.h"
#include "Timer.h"
#include "Image.h"
#include "Font.h"

class Game {
public:
	Game();
	~Game();

	Timer* m_timer;
	Image* m_imageMgr;
	Font* m_fontMgr;

private:

};
#endif // !_GAME_H_
