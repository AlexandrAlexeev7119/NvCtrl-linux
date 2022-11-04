## Version 1.4.0 (release)
### Changes and fixes
* Big updates:
* * Fixed bugs in UpdateChecker
* * Changes all urls, on dialog about fixed label alignment
* * Added some new methods to MainWindow class for update clock offset values
* * Reimplemented clock profile UI and internals (NOT compatible with older versions!!!)
* * Reviewed and Reimplemented some methods inside GpuClockController class
* * In ClockProfileDialog replaced raw for-loops with std::for_each(), improved readability
* * Optimized and fixed applying of attributes via nvidia-settings subprocess call (packs args to batch instead of creating of nvidia-settings instances)
* * Fixed error when trying to open unexisting file in home dir (now SettingsManager create a new file)
* * Removed support of loading default config from /usr/share/gwepp/gwepp.json
* PKGBUILD files is no longer distributes

## Version 1.3.19 (pre-release (UNSTABLE))
### Changes and fixes
* Replaced submodule nlohman-json with single-include header file to speedup PKGBUILD packaging
* Fixed compile error

## Version 1.3.18 (pre-release (UNSTABLE))
### Changes and fixes
* Fixed bug in UpdateChecker
* Some internal changes and fixes

## Version 1.3.16 (pre-release (UNSTABLE))
### Changes and fixes
* Changes all urls, on dialog about fixed label alignment

## Version 1.3.15 (pre-release (UNSTABLE))
### Changes and fixes
* Added support to show clock offset values of current profile
* Fixed error with old (wrong) values on window when canceling EditClockOffsetProfile dialog
* Added some new methods to MainWindow class for update clock offset values

## Version 1.3.14 (pre-release (UNSTABLE))
### Changes and fixes
* Added current clock profile editting support (renewed EditClockOffsetProfileDialog UI)
* Fixed layout and widgets width on ClockProfileDialog
* In ClockProfileDialog replaced raw for-loops with std::for_each(), improved readability
* Some other fixes

## Version 1.3.11 (pre-release (UNSTABLE))
### Changes and fixes
* Fixed error with unexisting directory in /home

## Version 1.3.10 (pre-release (UNSTABLE))
### Changes and fixes
* Fixed error when trying to open unexisting file in home dir (now SettingsManager create a new file)
* Removed support of loading default config from /usr/share/gwepp/gwepp.json
* Updated default settings and fixed error with wrong value

## Version 1.3.9 (pre-release (UNSTABLE))
### Changes and fixes
* Reimplemented clock profile saving
* Added support clocks for GPU and memory both
* Added support of applying custon clock profile
* Reviewed and Reimplemented some methods inside GpuClockController class
* Optimized and fixed applying of attributes via nvidia-settings subprocess call (packs args to batch instead of creating of nvidia-settings instances)

## Version 1.3.8 (pre-release (UNSTABLE))
### Changes and fixes
* Added support of saving and applying clock profiles
* Internal changes in GpuClockController class (added new methods and other)
* Removed warning when selecting custom clock profile

## Version 1.3.7 (pre-release (UNSTABLE))
### Changes and fixes
* Started develop of new clock profile design (per pstate control)
* Show messageBox with warning - creating new clock profile is not implemented yet (it will implemented in next ver)

## Version 1.3.6 (stable release)
### Changes and fixes
* Updates submodules (update NVMLpp: PIMPL idiom)
* Some other fixes

## Version 1.3.5 (stable release)
### Changes and fixes
* Fixed error with thread stopping while it running
* Added new signal in UpdateChecker
* Showing message box when no updates found

## Version 1.3.4 (stable release)
### Changes and fixes
* Updated logic inside SettingsManager
* Fixed std::move() call in main.cpp (remove const)

## Version 1.3.3 (stable release)
### Changes and fixes
* Updated public interface of SettingsManager class
* Moved app settings validation from MainWidnow into SettingsManager
* Some other internal fixes

## Version 1.3.2 (stable release)
### Changes and fixes
* Added qrc file and move icons inside it (no more /usr/share/icons is used)
* Fixed error with missing icons when run AppImage
* Updated PKGBUILD for Arch based distros

## Version 1.3.1 (stable release)
### Changes and fixes
* Hotfix: fixed error with settings (reading null object)
* Added Appimage

## Version 1.3.0 (stable release)
### Changes and fixes
* Added check for updates from both branches
* Added changelog to repository
* Added support to read changelog when new version is available
* Internal changes (codestyle, logic)
* Update default app settings

## Version 1.2.8 (pre-release)
### Changes and fixes
* Added changelog to repository
* Added support to read changelog when new version is available
* Internal changes (codestyle, logic)

## Version 1.2.7 (pre-release)
### Changes and fixes
* Added support for check updates at startup
* UpdateChecker now is non-blocking thread (QThread), no blocks at startup
* Updated default app settings

