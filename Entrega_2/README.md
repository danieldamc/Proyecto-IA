# Entrega 2 - Proyecto Inteligencia Artificial
## Car sequencing problem (CS) - Backtracking

Nombre: Daniel Martinez Castro  
Rol: 201973508-k

## Instrucciones de uso
### Compilacion
Para compilar el programa se utiliza:
```bash
make programa
```
o simplemente:
```bash
make
```
### Ejecucion
Para ejecutar el progama se utiliza:
```bash
make run ARGS="<testfile>"
```
en donde <testfile> corresponde al archivo de entrada. Por ejemplo para ejecutar el programa con un archivo llamado test.txt se utilizaria, NOTESE que el .txt no se introduce:
```bash
make run ARGS="test"
```
### Clean
Para eliminar los archivos generados por la compilacion y ejecucion se utiliza:
```bash
make clean
``` 
## Consideraciones
Como se explicita en la presentacion del problema el output debe contener las numero de violaciones a cada una de las opciones por lo tanto se asume que el algoritmo debe guardar una solucion candidata en el caso de que no encuentre solucion, lo cual obliga a relajar la restriccion de capacidad de las estaciones de trabajo, por lo tanto obligando al programa a recorrer todo el espacio de busqueda haciendo el programa extremadamente impractico para instancias del problema con un numero de autos mayor a 20, instancias de este tamaño o mas grande pueden llevar meses o incluso años en ejecutarse.  

Este programa retorna la primera solucion que encuentra, o la con menos violaciones en el caso de que no exista una solucion.  

Debido a los altos tiempos de ejecucion de este programa tambien se guarda en el .out la solucion mas optima encontrada en la ejecucion cada 1000000 de intentos de soluciones posibles.  

Las instancias ejemplo que se dieron son imposibles de ejecutar con este metodo ya que toman un tiempo extremadamente alto debido a la explosion combinacional del CS, incluso mayor a la cantidad de años del universo. y esto solo para la instancia mas pequeña de 200 autos.

## Versiones
gcc 9.4.0  
SO Ubuntu 20.04.01 LTS  