#############################################################################
# Makefile for building: KImageMerge
# Generated by qmake (3.0) (Qt 5.5.0)
# Project:  KImageMerge.pro
# Template: app
# Command: D:\Qt\Qt5.5.0\5.5\mingw492_32\bin\qmake.exe -spec win32-g++ -o Makefile KImageMerge.pro
#############################################################################

MAKEFILE      = Makefile

first: release
install: release-install
uninstall: release-uninstall
QMAKE         = D:\Qt\Qt5.5.0\5.5\mingw492_32\bin\qmake.exe
DEL_FILE      = del
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
COPY          = copy /y
COPY_FILE     = copy /y
COPY_DIR      = xcopy /s /q /y /i
INSTALL_FILE  = copy /y
INSTALL_PROGRAM = copy /y
INSTALL_DIR   = xcopy /s /q /y /i
DEL_FILE      = del
SYMLINK       = $(QMAKE) -install ln -f -s
DEL_DIR       = rmdir
MOVE          = move
SUBTARGETS    =  \
		release \
		debug


release: FORCE
	$(MAKE) -f $(MAKEFILE).Release
release-make_first: FORCE
	$(MAKE) -f $(MAKEFILE).Release 
release-all: FORCE
	$(MAKE) -f $(MAKEFILE).Release all
release-clean: FORCE
	$(MAKE) -f $(MAKEFILE).Release clean
release-distclean: FORCE
	$(MAKE) -f $(MAKEFILE).Release distclean
release-install: FORCE
	$(MAKE) -f $(MAKEFILE).Release install
release-uninstall: FORCE
	$(MAKE) -f $(MAKEFILE).Release uninstall
debug: FORCE
	$(MAKE) -f $(MAKEFILE).Debug
debug-make_first: FORCE
	$(MAKE) -f $(MAKEFILE).Debug 
debug-all: FORCE
	$(MAKE) -f $(MAKEFILE).Debug all
debug-clean: FORCE
	$(MAKE) -f $(MAKEFILE).Debug clean
debug-distclean: FORCE
	$(MAKE) -f $(MAKEFILE).Debug distclean
debug-install: FORCE
	$(MAKE) -f $(MAKEFILE).Debug install
debug-uninstall: FORCE
	$(MAKE) -f $(MAKEFILE).Debug uninstall

Makefile: KImageMerge.pro ../../Qt5.5.0/5.5/mingw492_32/mkspecs/win32-g++/qmake.conf ../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/spec_pre.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/qdevice.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/device_config.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/qconfig.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dcore.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dcore_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dinput.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dinput_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dquick.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dquick_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dquickrenderer.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dquickrenderer_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3drenderer.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3drenderer_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axbase.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axbase_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axcontainer.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axcontainer_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axserver.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axserver_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_bluetooth.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_bluetooth_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_bootstrap_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_clucene_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_concurrent.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_concurrent_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_core.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_core_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_dbus.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_dbus_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_declarative.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_declarative_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_designer.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_designer_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_designercomponents_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_enginio.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_enginio_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_gui.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_gui_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_help.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_help_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_location.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_location_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_multimedia.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_multimedia_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_multimediawidgets.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_multimediawidgets_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_network.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_network_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_nfc.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_nfc_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_opengl.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_opengl_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_openglextensions.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_openglextensions_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_platformsupport_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_positioning.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_positioning_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_printsupport.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_printsupport_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qml.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qml_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qmldevtools_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qmltest.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qmltest_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_quick.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_quick_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_quickparticles_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_quickwidgets.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_quickwidgets_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_script.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_script_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_scripttools.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_scripttools_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_sensors.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_sensors_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_serialport.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_serialport_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_sql.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_sql_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_svg.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_svg_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_testlib.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_testlib_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_uiplugin.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_uitools.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_uitools_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webchannel.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webchannel_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webkit.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webkit_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webkitwidgets.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webkitwidgets_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_websockets.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_websockets_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_widgets.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_widgets_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_winextras.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_winextras_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_xml.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_xml_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_xmlpatterns.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_xmlpatterns_private.pri \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/qt_functions.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/qt_config.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/win32/qt_config.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/win32-g++/qmake.conf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/spec_post.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/exclusive_builds.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/default_pre.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/win32/default_pre.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/resolve_config.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/exclusive_builds_post.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/default_post.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/c++11.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/win32/rtti.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/precompile_header.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/warn_on.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/qt.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/resources.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/moc.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/win32/opengl.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/uic.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/win32/windows.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/testcase_targets.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/exceptions.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/yacc.prf \
		../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/lex.prf \
		KImageMerge.pro \
		D:/Qt/Qt5.5.0/5.5/mingw492_32/lib/qtmain.prl \
		D:/Qt/Qt5.5.0/5.5/mingw492_32/lib/Qt5Widgets.prl \
		D:/Qt/Qt5.5.0/5.5/mingw492_32/lib/Qt5Gui.prl \
		D:/Qt/Qt5.5.0/5.5/mingw492_32/lib/Qt5Core.prl
	$(QMAKE) -spec win32-g++ -o Makefile KImageMerge.pro
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/spec_pre.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/qdevice.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/device_config.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/qconfig.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dcore.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dcore_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dinput.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dinput_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dquick.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dquick_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dquickrenderer.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3dquickrenderer_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3drenderer.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_3drenderer_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axbase.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axbase_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axcontainer.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axcontainer_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axserver.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_axserver_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_bluetooth.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_bluetooth_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_bootstrap_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_clucene_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_concurrent.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_concurrent_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_core.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_core_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_dbus.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_dbus_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_declarative.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_declarative_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_designer.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_designer_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_designercomponents_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_enginio.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_enginio_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_gui.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_gui_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_help.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_help_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_location.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_location_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_multimedia.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_multimedia_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_multimediawidgets.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_multimediawidgets_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_network.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_network_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_nfc.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_nfc_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_opengl.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_opengl_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_openglextensions.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_openglextensions_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_platformsupport_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_positioning.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_positioning_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_printsupport.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_printsupport_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qml.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qml_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qmldevtools_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qmltest.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qmltest_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_qtmultimediaquicktools_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_quick.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_quick_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_quickparticles_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_quickwidgets.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_quickwidgets_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_script.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_script_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_scripttools.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_scripttools_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_sensors.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_sensors_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_serialport.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_serialport_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_sql.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_sql_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_svg.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_svg_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_testlib.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_testlib_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_uiplugin.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_uitools.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_uitools_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webchannel.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webchannel_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webkit.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webkit_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webkitwidgets.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_webkitwidgets_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_websockets.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_websockets_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_widgets.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_widgets_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_winextras.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_winextras_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_xml.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_xml_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_xmlpatterns.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/modules/qt_lib_xmlpatterns_private.pri:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/qt_functions.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/qt_config.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/win32/qt_config.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/win32-g++/qmake.conf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/spec_post.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/exclusive_builds.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/default_pre.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/win32/default_pre.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/resolve_config.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/exclusive_builds_post.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/default_post.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/c++11.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/win32/rtti.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/precompile_header.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/warn_on.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/qt.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/resources.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/moc.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/win32/opengl.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/uic.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/win32/windows.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/testcase_targets.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/exceptions.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/yacc.prf:
../../Qt5.5.0/5.5/mingw492_32/mkspecs/features/lex.prf:
KImageMerge.pro:
D:/Qt/Qt5.5.0/5.5/mingw492_32/lib/qtmain.prl:
D:/Qt/Qt5.5.0/5.5/mingw492_32/lib/Qt5Widgets.prl:
D:/Qt/Qt5.5.0/5.5/mingw492_32/lib/Qt5Gui.prl:
D:/Qt/Qt5.5.0/5.5/mingw492_32/lib/Qt5Core.prl:
qmake: FORCE
	@$(QMAKE) -spec win32-g++ -o Makefile KImageMerge.pro

qmake_all: FORCE

make_first: release-make_first debug-make_first  FORCE
all: release-all debug-all  FORCE
clean: release-clean debug-clean  FORCE
distclean: release-distclean debug-distclean  FORCE
	-$(DEL_FILE) Makefile

release-mocclean:
	$(MAKE) -f $(MAKEFILE).Release mocclean
debug-mocclean:
	$(MAKE) -f $(MAKEFILE).Debug mocclean
mocclean: release-mocclean debug-mocclean

release-mocables:
	$(MAKE) -f $(MAKEFILE).Release mocables
debug-mocables:
	$(MAKE) -f $(MAKEFILE).Debug mocables
mocables: release-mocables debug-mocables

check: first
FORCE:

$(MAKEFILE).Release: Makefile
$(MAKEFILE).Debug: Makefile
