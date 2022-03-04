project(LemonAudio)

set(lemon_audio_SRC
	src/audio_player.cpp
	src/sound_context.cpp
	src/sound.cpp
	src/sound_source.cpp
)
add_library(lemon_audio ${lemon_audio_SRC})
add_library(lemon::audio ALIAS lemon_audio)

target_include_directories(lemon_audio PUBLIC include/)

target_link_libraries(lemon_audio
	PUBLIC
	lemon::core
	lemon::assets
	PRIVATE
	${OPENAL_LIBRARY}
	AudioFile
)