## Version 1.4.13 (release)
### Changes and fixes
* Added basic processes view (PID, memory usage, proc name)
* Added download links to changelog
* Open browser when link clicked on RecentUpdatesDialog window
* Removed std::async() call, replace by std::for_each
* Fixed not showing messagebox when local version is little bit higher than remote (for developers)
* Fixed wrong fan profile loading (it was load to clock offset comboBox widget)
* Removed unused code
### Download
Download AppImage file [for version 1.4.13](https://codeberg.org/AlexCr4ckPentest/GWEpp/releases/tag/v1.4.13)

## Version 1.4.12 (pre-release (UNSTABLE))
### Changes and fixes
* Fixed wrong fan profile loading (it was load to clock offset comboBox widget)
* Removed unused code
### Download
Download AppImage file [for version 1.4.12](https://codeberg.org/AlexCr4ckPentest/GWEpp/releases/tag/v1.4.12)

## Version 1.4.11 (pre-release (UNSTABLE))
### Changes and fixes
* Removed std::async() call, replace by std::for_each
* Fixed: wrong memory usage value for running processes
### Download
Download AppImage file [for version 1.4.11](https://codeberg.org/AlexCr4ckPentest/GWEpp/releases/tag/v1.4.11)

## Version 1.4.10 (pre-release (UNSTABLE))
### Changes and fixes
* Added selected process termination via kill() syscall
* Added download links to changelog
* Open browser when link clicked on RecentUpdatesDialog window
### Download
Download AppImage file [for version 1.4.10](https://codeberg.org/AlexCr4ckPentest/GWEpp/releases/tag/v1.4.10)

## Version 1.4.9 (pre-release (UNSTABLE))
### Changes and fixes
* Added base for killing selected process
* Added spacer in menuBar at tab 'view'
* Replaced GpuProcessesOverviewDialog with raw NVMLpp::NVML_device\* (GpuProcessesController not used yet)
### Download
Download AppImage file [for version 1.4.9](https://codeberg.org/AlexCr4ckPentest/GWEpp/releases/tag/v1.4.9)

## Version 1.4.8 (pre-release (UNSTABLE))
### Changes and fixes
* Added basic processes view (PID, memory usage, proc name)
* Updated UI elements on GpuProcessesOverviewDialog
* Fixed errors in NVMLpp lib
### Download
Download AppImage file [for version 1.4.8](https://codeberg.org/AlexCr4ckPentest/GWEpp/releases/tag/v1.4.8)

## Version 1.4.7 (pre-release (UNSTABLE))
### Changes and fixes
* Changed class name
* Added new class GpuProcessesController to get info about running processes on GPU
* Added timer on GpuProcessesOverviewDialog for work with GpuClockController
* Fixed not showing messagebox when local version is little bit higher than remote (for developers)
* Some other internal changes (codestyle, logic)
### Download
Download AppImage file [for version 1.4.7](https://codeberg.org/AlexCr4ckPentest/GWEpp/releases/tag/v1.4.7)

## Version 1.4.5 (release)
### Changes and fixes
* Added support only one single application instance
* Added support to show MainWindow of already running application via IPC (DBus)
* New dependency: QtDBus lib (should be already installed with qt5-base package (for Arch GNU/Linux), other distros not tested)
* Some other internal fixes
### Download
Download AppImage file [for version 1.4.5](https://codeberg.org/AlexCr4ckPentest/GWEpp/releases/tag/v1.4.5)

## Version 1.4.2 (release)
### Changes and fixes
* Changed SettingsManager class, now its doesn\`t need QObject
* Removed std::unique_ptr<> from SettingsManager for std::fstream object
### Download
Download AppImage file [for version 1.4.2](https://codeberg.org/AlexCr4ckPentest/GWEpp/releases/tag/v1.4.2)

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
### Download
Download AppImage file [for versions 1.4.0](https://codeberg.org/AlexCr4ckPentest/GWEpp/releases/tag/v1.4.0)

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

