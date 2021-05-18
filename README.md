# Material-Archiv
Dieses Archiv enthält alle nötigen Materialien für die praktische Aufgabe. 
Dazu gehören Benchmark-Instanzen sowie Einleseroutinen für Java und C++. 

Die Parameter des Programms sollen folgendermaßen sein:
~~~~
# Wenn C++
./solver <instance> <solution> <timelimit> <seed>
# Wenn Java
java -jar Solver.jar <instance> <solution> <timelimit> <seed>
~~~~



Profiling Software
---------------------

### Java

Die vorgestellte Software heißt [VisualVM](https://visualvm.github.io/).
In IntelliJ IDEA kann VisualVM einfach mit dem
Plugin [VisualVM-Launcher](https://plugins.jetbrains.com/plugin/7115-visualvm-launcher) eingebunden werden.

Ansonsten kann VisualVM auch aus dem Menü gestartet werden.
Wenn dann der Solver gestartet wird, sollte in der Auswahlliste auf der linken Seite des VisualVM-Fensters
der entsprechende Prozess anzeigt werden.


### C++

Für C++ steht mit [Valgrind](http://www.valgrind.org/) eine ganze Menge von Werkzeugen
zur Verfügung.

Für Instruction-Level-Profiling kann das Werkzeug `callgrind` verwendet werden.
Für Heap-Memory-Profiling steht das Werkzeug `massif` zur Verfügung.


**Beispiel**

~~~
valgrind --tool=callgrind ./my-solver my-instance best.txt 10 1337
valgrind --tool=massif ./my-solver my-instance best.txt 10 1337
~~~

Die Werkzeuge erzeugen Ausgabedateien `callgrind.out.x` und `massif.out.x` wobei `x` eine Zahl ist.
Diese Ausgabedateien können mit den Programmen `kcachegrind` und `massif-visualizer` angezeigt werden.

Auf Ubuntu-Linux stehen die Pakete `kcachegrind`, `massif-visualizer` und `valgrind` via `apt` zur Verfügung.

