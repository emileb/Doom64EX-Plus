
LOCAL_PATH := $(call my-dir)/../


include $(CLEAR_VARS)

LOCAL_MODULE    := doom64ExPlus_5

LOCAL_CFLAGS := -DENGINE_NAME=\"doom64\" -Dstricmp=strcasecmp -fsigned-char -DDOOM64EX


LOCAL_C_INCLUDES :=     $(SDL3_INCLUDE_PATHS)  \
                        $(TOP_DIR) \
                        $(TOP_DIR)/MobileTouchControls \
                        $(TOP_DIR)/Clibs_OpenTouch \
                        $(TOP_DIR)/Clibs_OpenTouch/idtech1 \
                        $(LOCAL_PATH)/mobile  \
                        $(TOP_DIR)/gl4es/include \
                        $(TOP_DIR)/AudioLibs_OpenTouch/fluidsynth/include\
                        $(TOP_DIR)/AudioLibs_OpenTouch/FMOD_studio/api/lowlevel/inc \

LOCAL_SRC_FILES =   ../../../../Clibs_OpenTouch/idtech1/android_jni.cpp \
                    ../../../../Clibs_OpenTouch/idtech1/touch_interface.cpp \
                    mobile/game_interface.c \
    i_system.c \
 	am_draw.c \
 	am_map.c \
 	info.c \
 	kpf.c \
 	md5.c \
 	tables.c \
 	con_console.c \
 	con_cvar.c \
 	d_devstat.c \
 	d_main.c \
 	d_net.c \
 	f_finale.c \
 	in_stuff.c \
 	g_actions.c \
 	g_demo.c \
 	g_game.c \
 	g_settings.c \
 	wi_stuff.c \
 	m_cheat.c \
 	m_menu.c \
 	m_misc.c \
 	m_fixed.c \
 	m_keys.c \
 	m_password.c \
 	m_random.c \
 	m_shift.c \
 	net_client.c \
 	net_common.c \
 	net_io.c \
 	net_loop.c \
 	net_packet.c \
 	net_query.c \
 	net_server.c \
 	net_structure.c \
 	dgl.c \
 	gl_draw.c \
 	gl_main.c \
 	gl_texture.c \
 	sc_main.c \
 	p_ceilng.c \
 	p_doors.c \
 	p_enemy.c \
 	p_user.c \
 	p_floor.c \
 	p_inter.c \
 	p_lights.c \
 	p_macros.c \
 	p_map.c \
 	p_maputl.c \
 	p_mobj.c \
 	p_plats.c \
 	p_pspr.c \
 	p_saveg.c \
 	p_setup.c \
 	p_sight.c \
 	p_spec.c \
 	p_switch.c \
 	p_telept.c \
 	p_tick.c \
 	r_clipper.c \
 	r_drawlist.c \
 	r_lights.c \
 	r_main.c \
 	r_scene.c \
 	r_bsp.c \
 	r_sky.c \
 	r_things.c \
 	r_wipe.c \
 	s_sound.c \
 	st_stuff.c \
 	i_audio.c \
 	i_main.c \
 	i_png.c \
 	i_video.c \
 	w_file.c \
 	w_merge.c \
 	w_wad.c \
 	z_zone.c \
 	i_sdlinput.c \
 	sha1.c \
 	steam.c \
 	p_mapinfo.c \
 	i_shaders.c \
 	i_sectorcombiner.c \


LOCAL_LDLIBS += -llog -lz #-lGLESv1_CM

LOCAL_STATIC_LIBRARIES += SDL2_net logwritter

LOCAL_SHARED_LIBRARIES := touchcontrols fluidsynth SDL3 SDL2_mixer SDL2_image core_shared saffal GL4ES fmod

#Strip unused functions/data
LOCAL_CFLAGS += -funsafe-math-optimizations -fvisibility=hidden -fdata-sections -ffunction-sections -fPIC


include $(BUILD_SHARED_LIBRARY)






