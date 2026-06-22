# Change Log

All notable changes to this project will be documented in this file.
 
The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [0.0.4-alpha] -

### Added

 - Added displayMove
 - Added .vscode config
 - Added link Makefile target
 - .clang-tidy suppress modernize-macro-to-enum
 - Added ScoredMove struct
 - Added alpha-beta pruning to search
 - Added piece value definitions
 - Added move scoring and ordering
 - Added quiescentSearch
 - Added static position evaluation
 - Added move display
 - Added pawn penalties

### Changed

 - Renamed 'engineBestMove' to 'search'
 - Moved logging to ~/.local/state/chess-engine/

## [0.0.3-alpha] - 2026-06-17

### Added

 - Added MOVE_PROMOTION move flag.
 
### Changed

 - Updated boardMakeMove to filter king and pawn move logic by piece type.
 - Updated boardUnmakeMove to use MOVE_PROMOTION.
  
## [0.0.2-alpha] - 2026-06-16

### Fixed

 - Fixed boardMakeMove pawn promotion handling.
 - Fixed boardUnmakeMove pawn promotion handling.
