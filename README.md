# emi-extractor
Extractor for .EMI Files

## API
no docs yet

## Frontend

### Build
Just run `make` but make sure you have GCC because that's what I use.

### Frontend
```emi_extractor.exe [SOURCE] [OUTPUT]```  
This will separate all of the sections in the source file into multiple files with the prefix specified by `[OUTPUT]`.  
If `[OUPUT]` is not provided then the default prefix is the filename. Except it doesn't do that and the default prefix
is always the filename because I forgot to make those changes.
