#ifndef _BODYPROG_GAMEBOOT_BACKGROUNDSOUNDINIT_H
#define _BODYPROG_GAMEBOOT_BACKGROUNDSOUNDINIT_H

/** @brief This header is used to declare any variable, struct, or
 * function part of `BODYPROG.BIN` identified to be related
 * to the functionallity and logic of the main background music
 * initalization and some track update.
 *
 * @note This code generally handle the change or initalization of
 * music and enviromental sounds when moving through the map.
 * It's `Sd_BgmInit` which is in charge of that behaviour of updating
 * the music when going through rooms or maps. Same with `Sd_AmbientSfxInit`,
 * but with enviromental sound effects.
 */

// ==========
// FUNCTIONS
// ==========

/** @brief Initializes background music or updates current track.
 *
 * @return Returns -1 if something is being loaded in memory or if
 * some audio is being play. Returns 0 if music has been muted or
 * updated, returns 1 if everything fails.
 *
 * @note In case of returning 0 for the only instance this function
 * is used the main loading screen will be triggered.
 */
s32 Sd_BgmInit(void);

/** @brief Checks if a given background music track is not currently playing
 * or if it is a command to trigger the update of layers or track change.
 *
 * @param bgmIdx Index of the background music track to check.
 * @return `true` if the given track is not currently playing, `false` otherwise
 * and in case of being an update layer/track command.
 */
bool Sd_BgmActiveSongCheck(s32 bgmIdx);

/** @brief Updates track.
 *
 * @param bgmIdx `e_BgmCmd`.
 * @note This function is used to change tracks in between map/room
 * changes or when loading a save only in case the overlay uses multiple
 * tracks.
 */
void Sd_BgmSongSet(s32 bgmIdx);

/** @brief Updates track in case `g_MapOverlayHdr.bgmCmd` is defined as
 * `BgmCmd_UpdateTrack`.
 *
 * @note This function is used to change tracks in between map/room
 * changes or when loading a save only in case the overlay uses multiple
 * tracks.
 */
void Sd_BgmUpdateTrack(void);

/** @brief Update channels being used by songs.
 * @todo Properly documment after investigating game's channel functionallity.
 */
void Sd_BgmChannelSet(void);

/** @brief Initializes enviromental audios or updates current audio track.
 *
 * @return Returns -1 if something is being loaded in memory or if
 * some audio is being play. Returns 0 if nothing has been updated,
 * and returns 1 if the audio has been updated.
 */
s32 Sd_AmbientSfxInit(void);

/** @brief Checks if a given background audio is not currently playing.
 *
 * @param ambientIdx Index of the background audio to check.
 * @return `true` if the given audio is not currently playing, `false` otherwise.
 */
bool Sd_ActiveAmbientSfxCheck(s32 ambientIdx);

/** @brief Updates track.
 *
 * @param idx Index of the background audio to change.
 */
void Sd_AmbientSfxSet(s32 idx);

#endif
