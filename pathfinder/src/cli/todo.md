- [x] eliminate trailing blank lines from output
- [x] eliminate leading blank lines from output
- [x] keep indentation even when blank lines
- [x] backspace, make sure all are spaces not just previous 4
- [x] backspace, always delete 4 spaces
- [x] add mouse wheel scrolling to input window
- [x] feedback when executing query
- [x] remove "Executing query..." on error
- [x] Ctrl+O: clear output
- [x] save history to file
- [x] load history from file
- [x] fix vertical scroll off last and first line
- [x] remove trailing and leading lines with only spaces from output
- [x] show output in table, using tsv
- [x] show pager output in table
- [x] better error messages:
      ```
      select * {
               ^
               error bla bla
      ```
      to allow this add line and column to QueryParsingException.
- [x] show result count
- [x] Show during execution: Executing query -> Calculating column width -> Generating table
- [x] Add MQL tsv output so that the table is created correctly
- [x] Check less pager exit status
- [x] Control + Backspace
- [x] Left/Right at end of line should move line up/down
- [x] Don't automatically show pager
- [x] Cyan PF> input prompt
- [x] Show how to send query
- [x] Add ncurses and less dependency to
      - .deb control files
      - docker builder images
      - compile instructions
- [x] Allow user to interrupt queries
- [x] Query execution times
- [x] Join line
- [x] Delete line
- [x] Remove progress spinner flicker (don't show is query takes less than 100ms)
- [x] On join line remove trailing and leading whitespace
- [x] Only show "Results in TSV format" when user interrupts table generation
- [x] On timeout or Ctrl+C show partial results, generate table anyway
- [x] On timeout show "Timeout after x seconds"


Maybe/Future
============
- [ ] Commands: .clear, .limit 20, etc
- [ ] If line starts with . (internal command), on enter send immediately without Ctrl+S
- [ ] Limit output, ring buffer
- [ ] Send queries on enter if ending in a terminator char.
      Can not use ';', ',' or '.' because they are valid lines endings for SPARQL queries.