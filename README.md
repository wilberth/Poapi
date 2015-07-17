# Poapi
Python module and shell interface for the NDI Optotrak API

## Python usage
Example usage of the module:
```python
#!/usr/bin/env python
import poapi

optotrak = poapi.Poapi()
print optotrak.OAPIGetVersionString()
```

a fully functional script:
```python
#!/usr/bin/env python
import poapi

optotrak = poapi.Poapi()
optotrak.TransputerInitializeSystem()

```



Usually the python functions are called the same way as the original C functions. Sometimes
we use the additional capabilities of Python. A few examples:

C:
```c
char s[1024]
OptoGetLastError(s, 1024)
// nu bevat s de error string
```
Python:
```python
s = OptoGetLastError()
```

C:
```c
float a = 1.0, b = 2.0;
OdauSetAnalogOutputs(ODAU2, a, b, 0)
// nu zijn  a en b gewijzigd in de nieuwe waarden, bijvoorbeeld 3.0 en 4.0
```

Python:
```
a, b = OdauSetAnalogOutputs(2, 1.0, 2.0, 0) 
```

## Shell usage
You can also use the shell directly, without python. Execute the *poapi* program from the 
command line and start typing your functions. Function output will appear on the next line.

```bash
terra:Poapi> ./poapi
TransputerDetermineSystemCfg()
0
TransputerLoadSystem()
0
```

## Ethernet
An additional script is present to find the ip-address of the optotrak scu for use in the optotrak ini file:
```bash
terra:Poapi> ./findScu.pl
scan range: 131.174.111.26/24 for Northern Digital
Northern Digital SCU, IP: 131.174.111.1, MAC: e8:39:35:39:03:d1
```
