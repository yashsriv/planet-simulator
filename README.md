# Planet-Simulator

![Screenshot](https://raw.githubusercontent.com/yashsriv/planet-simulator/screen.png)

A simple simulation for bodies in a game field obeying the laws of physics.

You can draw this in different modes - potential map mode as well as planet mode.

```
Usage:   simulator [-option] [argument]
options: -h                      show help information(this output)
         -n number               number of planets
         -m (planet | potential) draw either planets or potential.
         -g[gravity-factor=1]    enable gravity. You can set the factor too(optional) like -g1.2 - no spaces.
         -l layers               number of intensity layers. Not yet implemented
         -f fps                  set fps. This is not actual fps but rather the supposed fps by the program.
                                   For example: If fps=1000 output is at 60fps assuming that output is 1000fps.
                                   Can be used for better calculations
         -d                      debug mode - lots of output
         -x                      extreme mode - continuous output

example: simulator -n 2 -d potential -g0 -l 10 -f 30

         ^ The above example is the default incase any one of the options are not specified
```
