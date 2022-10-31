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

