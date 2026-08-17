#ifndef _BODYPROG_EVENTS_BGMUTILS_H
#define _BODYPROG_EVENTS_BGMUTILS_H

/** @brief This header contains reference to functions used to
 * provide some effect or handling over BGM.
 *
 * @note Most of this functions are unused and even the used one
 * is only rarely used in the last overlays.
 */
 
/** @brief Plays a new background song. If the song is already playing, the request is ignored.
 *
 * @param bgmIdx Index of the background song to play.
 */
void Bgm_PlayNewSong(s32 bgmIdx);

/** @brief Handles a multi-step crossfade to a new song.
 *
 * @param bgmIdx Index of the background song to play.
 */
void Bgm_CrossfadeToTrack(s32 bgmIdx);

/** @brief Crossfades the currently playing background song to silence. */
void Bgm_CrossfadeToSilence(void);

/** @brief @unused Stop current song inmediately. */
void Bgm_SongStopImmediately(void);

/** @brief @unused Stop current song with fading.
 *
 * @param slowerFade Use slower fade.
 */
void Bgm_SongStopFadeout(bool slowerFade);

#endif