#include <iostream>     // Librer�a para operaciones de entrada y salida (cin, cout)
#include <string>       // Librer�a para usar la clase string

using namespace std;    // Permite usar nombres est�ndar sin anteponer "std::"

// Definici�n de estructura para representar un proceso
struct Proceso {
    int pid;                // Identificador �nico del proceso
    string nombre;          // Nombre del proceso
    int prioridad;          // Nivel de prioridad del proceso
    Proceso* siguiente;     // Puntero al siguiente proceso (lista enlazada)
};

// Puntero global al inicio de la lista de procesos
Proceso* cabezaProcesos = NULL;

// Estructura para nodo de la cola de planificaci�n
struct NodoCola {
    Proceso* proceso;       // Puntero al proceso contenido
    NodoCola* siguiente;    // Puntero al siguiente nodo en la cola
};

// Puntero global al inicio de la cola
NodoCola* cabezaCola = NULL;

// Estructura para representar bloques de memoria asignados a un proceso
struct BloqueMemoria {
    Proceso* proceso;        // Apuntador al proceso due�o del bloque
    int tamanio;             // Tama�o del bloque de memoria
    BloqueMemoria* siguiente; // Apuntador al siguiente bloque en la pila
};

// Puntero al tope de la pila de memoria
BloqueMemoria* topeMemoria = NULL;

//---------------- FUNCIONES AUXILIARES ----------------//

// Busca un proceso por su PID en la lista enlazada
Proceso* buscarProcesoPorPID(int pid) {
    Proceso* actual = cabezaProcesos; // Comienza desde la cabeza de la lista
    while (actual != NULL) {
        if (actual->pid == pid)      // Si se encuentra el PID
            return actual;           // Retorna el puntero al proceso
        actual = actual->siguiente;  // Avanza al siguiente nodo
    }
    return NULL; // No se encontr� el proceso
}

// Verifica si un proceso con el PID ya est� en la cola de planificaci�n
bool estaEnCola(int pid) {
    NodoCola* actual = cabezaCola;   // Comienza desde la cabeza de la cola
    while (actual != NULL) {
        if (actual->proceso->pid == pid) // Si encuentra el PID en alg�n nodo
            return true;                 // Ya est� en la cola
        actual = actual->siguiente;      // Avanza al siguiente nodo
    }
    return false; // No se encontr� el proceso en la cola
}

//---------------- GESTOR DE PROCESOS ----------------//

// Inserta un nuevo proceso en la lista enlazada
void insertarProceso() {
    cout << "--- Insertar nuevo proceso ---\n";

    int pid;
    do {
        cout << "Ingrese PID (entero positivo): ";
        cin >> pid;                            // Solicita el PID
        if (pid <= 0) {
            cout << "Error: El PID debe ser un n�mero entero positivo.\n";
        } else if (buscarProcesoPorPID(pid) != NULL) {
            cout << "Error: Ya existe un proceso con ese PID.\n";
            pid = -1;                          // Repetir entrada si ya existe
        }
        cin.clear();                           // Limpia el estado de cin
    } while (pid <= 0);                        // Repite hasta que el PID sea v�lido

    string nombre;
    cin.ignore();                              // Limpia el buffer del salto de l�nea
    do {
        cout << "Ingrese nombre del proceso: ";
        getline(cin, nombre);                  // Lee el nombre completo con espacios
        if (nombre.empty()) {
            cout << "Error: La cadena no puede estar vac�a.\n";
        }
    } while (nombre.empty());                  // Repite hasta que el nombre no est� vac�o

    int prioridad;
    do {
        cout << "Ingrese prioridad (entero positivo): ";
        cin >> prioridad;
        if (prioridad <= 0) {
            cout << "Error: La prioridad debe ser un n�mero entero positivo.\n";
        }
        cin.clear();
    } while (prioridad <= 0);                  // Repite hasta que la prioridad sea v�lida

    Proceso* nuevo = new Proceso;              // Crea un nuevo proceso din�micamente
    nuevo->pid = pid;                          // Asigna los valores
    nuevo->nombre = nombre;
    nuevo->prioridad = prioridad;
    nuevo->siguiente = NULL;

    if (cabezaProcesos == NULL) {              // Si la lista est� vac�a
        cabezaProcesos = nuevo;                // El nuevo proceso es el primero
    } else {
        Proceso* actual = cabezaProcesos;
        while (actual->siguiente != NULL) actual = actual->siguiente;
        actual->siguiente = nuevo;             // Agrega el proceso al final de la lista
    }

    cout << "Proceso insertado correctamente.\n";
}

// Elimina un proceso de la lista enlazada por PID
void eliminarProceso() {
    cout << "--- Eliminar proceso ---\n";
    int pid;
    do {
        cout << "Ingrese PID a eliminar: ";
        cin >> pid;
        if (pid <= 0) {
            cout << "Error: Debe ingresar un n�mero entero positivo.\n";
        }
        cin.clear();
    } while (pid <= 0);                         // Repite hasta que el PID sea v�lido

    if (cabezaProcesos == NULL) {              // Lista vac�a
        cout << "No hay procesos para eliminar.\n";
        return;
    }

    if (cabezaProcesos->pid == pid) {          // El proceso a eliminar es el primero
        Proceso* temp = cabezaProcesos;
        cabezaProcesos = cabezaProcesos->siguiente;
        delete temp;                           // Libera la memoria
        cout << "Proceso eliminado.\n";
        return;
    }

    Proceso* actual = cabezaProcesos;
    while (actual->siguiente != NULL && actual->siguiente->pid != pid)
        actual = actual->siguiente;

    if (actual->siguiente != NULL) {           // Se encontr� el proceso
        Proceso* temp = actual->siguiente;
        actual->siguiente = temp->siguiente;   // Lo elimina de la lista
        delete temp;                           // Libera la memoria
        cout << "Proceso eliminado.\n";
    } else {
        cout << "Proceso no encontrado.\n";    // No se encontr� el PID
    }
}

// Muestra todos los procesos registrados
void mostrarProcesos() {
    cout << "--- Lista general de procesos ---\n";
    
    // Verifica si no hay procesos en la lista general
    if (cabezaProcesos == NULL) {
        cout << "No hay procesos registrados.\n";
        return;
    }

	// Recorre la lista de procesos e imprime sus datos
    Proceso* actual = cabezaProcesos;
    while (actual != NULL) {
        cout << "PID: " << actual->pid << ", Nombre: " << actual->nombre << ", Prioridad: " << actual->prioridad << "\n";
        actual = actual->siguiente; // Avanza al siguiente nodo
    }
}

//---------------- PLANIFICADOR DE CPU ----------------//

// Inserta un proceso en la cola ordenado por prioridad (menor n�mero = mayor prioridad)
void encolarProcesoEnPlanificador() {
    cout << "--- Encolar proceso en planificador ---\n";

    int pid;
    // Solicita el PID del proceso a encolar, validando que sea positivo
    do {
        cout << "Ingrese PID del proceso a encolar: ";
        cin >> pid;
        if (pid <= 0) {
            cout << "Error: Debe ingresar un n�mero entero positivo.\n";
        }
        cin.clear(); // Limpia posibles errores de entrada
    } while (pid <= 0);

    Proceso* p = buscarProcesoPorPID(pid);     // Busca el proceso en la lista general
    
    // Si no se encuentra, muestra error
    if (p == NULL) {
        cout << "Error: No existe un proceso con ese PID en la lista general.\n";
        return;
    }
	
	// Verifica si ya est� en la cola
    if (estaEnCola(pid)) {
        cout << "Error: El proceso ya est� en la cola de planificaci�n.\n";
        return;
    }
	
	// Crea un nuevo nodo de la cola
    NodoCola* nuevo = new NodoCola;     // Crea un nuevo nodo
    nuevo->proceso = p;					// Apunta al proceso encontrado
    nuevo->siguiente = NULL;			

    // Inserta el nodo ordenado por prioridad (menor valor = m�s prioritario)
    if (cabezaCola == NULL || cabezaCola->proceso->prioridad > p->prioridad) {
    	// Si la cola est� vac�a o el nuevo proceso tiene mayor prioridad
        nuevo->siguiente = cabezaCola;
        cabezaCola = nuevo;		// Se convierte en el nuevo primero
    } else {
        NodoCola* actual = cabezaCola;
        while (actual->siguiente != NULL && actual->siguiente->proceso->prioridad <= p->prioridad) {
            actual = actual->siguiente;		// Avanza mientras encuentre igual o menor prioridad
        }
        // Inserta el nuevo nodo en la posici�n adecuada
        nuevo->siguiente = actual->siguiente;
        actual->siguiente = nuevo;
    }

    cout << "Proceso encolado en planificador correctamente.\n";
}

// Desencola y "ejecuta" el primer proceso (mayor prioridad)
void desencolarYEjecutarProceso() {
	
    cout << "--- Desencolar y ejecutar proceso ---\n";
    
    // Verifica si la cola est� vac�a
    if (cabezaCola == NULL) {
        cout << "La cola de planificaci�n est� vac�a.\n";
        return;
    }

    NodoCola* temp = cabezaCola;				// Toma el primer nodo (mayor prioridad)
    cabezaCola = cabezaCola->siguiente;        // Avanza el inicio de la cola
	
	// Muestra informaci�n del proceso a ejecutar
    cout << "Ejecutando proceso:\n";
    cout << "PID: " << temp->proceso->pid << ", Nombre: " << temp->proceso->nombre << ", Prioridad: " << temp->proceso->prioridad << "\n";

    delete temp;	// Libera el nodo ejecutado
}

// Muestra la cola de planificaci�n
void mostrarColaPlanificador() {
	
    cout << "--- Cola de planificaci�n (ordenada por prioridad) ---\n";
    
    // Si est� vac�a, lo indica
    if (cabezaCola == NULL) {
        cout << "La cola de planificaci�n est� vac�a.\n";
        return;
    }
	
	// Recorre la cola e imprime cada proceso
    NodoCola* actual = cabezaCola;
    while (actual != NULL) {
        cout << "PID: " << actual->proceso->pid << ", Nombre: " << actual->proceso->nombre << ", Prioridad: " << actual->proceso->prioridad << "\n";
        actual = actual->siguiente;
    }
}


//---------------- GESTOR DE MEMORIA ----------------//

// Asigna memoria a un proceso (push a pila de memoria)
void asignarMemoria() {
    cout << "--- Asignar memoria a proceso ---\n";

    int pid;
    // Solicitar al usuario el PID del proceso al que se le asignar� memoria
    do {
        cout << "Ingrese PID del proceso: ";
        cin >> pid;
        // Validaci�n: El PID debe ser un n�mero entero positivo
        if (pid <= 0) {
            cout << "Error: Debe ingresar un numero entero positivo.\n";
        }
        cin.clear();  // Limpiar cualquier error en el buffer de entrada
    } while (pid <= 0);  // Repetir si el PID no es v�lido

    // Buscar el proceso por su PID en la lista de procesos
    Proceso* p = buscarProcesoPorPID(pid);
    if (p == NULL) {
        // Si el proceso no existe, mostrar un error
        cout << "Error: No existe un proceso con PID " << pid << " en la lista de procesos.\n";
        return;  // No se asigna memoria si el proceso no se encuentra
    }

    int tamanio;
    // Solicitar al usuario el tama�o de la memoria a asignar
    do {
        cout << "Ingrese tamano de memoria a asignar: ";
        cin >> tamanio;
        // Validaci�n: El tama�o de la memoria debe ser un n�mero entero positivo
        if (tamanio <= 0) {
            cout << "Error: Debe ingresar un numero entero positivo.\n";
        }
        cin.clear();  // Limpiar cualquier error en el buffer de entrada
    } while (tamanio <= 0);  // Repetir si el tama�o no es v�lido

    // Crear un nuevo bloque de memoria
    BloqueMemoria* nuevo = new BloqueMemoria;
    nuevo->proceso = p;  // Asociar el bloque de memoria con el proceso
    nuevo->tamanio = tamanio;  // Establecer el tama�o del bloque de memoria
    nuevo->siguiente = topeMemoria;  // Insertar al inicio de la pila
    topeMemoria = nuevo;  // El nuevo bloque es ahora el tope de la pila

    // Mensaje de confirmaci�n de la asignaci�n de memoria
    cout << "Memoria asignada al proceso " << pid << " (tamano: " << tamanio << ")\n";
}

// Libera el bloque superior de la pila de memoria (�ltimo asignado)
void liberarMemoria() {
    cout << "--- Liberar memoria ---\n";

    // Verificar si la pila de memoria est� vac�a
    if (topeMemoria == NULL) {
        cout << "No hay memoria para liberar.\n";
        return;  // No hay bloques de memoria para liberar
    }

    // Liberar el bloque superior de la pila de memoria (el �ltimo bloque asignado)
    BloqueMemoria* temp = topeMemoria;  // Guardar el bloque superior de la pila
    topeMemoria = topeMemoria->siguiente;  // Actualizar la cabeza de la pila (se mueve al siguiente bloque)

    // Mostrar informaci�n del proceso y el tama�o de la memoria liberada
    cout << "Memoria liberada del proceso con PID " << temp->proceso->pid
         << " (tamano: " << temp->tamanio << ")\n";
    
    // Liberar la memoria del bloque eliminado
    delete temp;  // Eliminar el bloque de memoria
}

// Muestra el estado actual de la memoria
void estadoMemoria() {
    cout << "--- Estado actual de la memoria ---\n";

    // Verificar si la pila de memoria est� vac�a
    if (topeMemoria == NULL) {
        cout << "La memoria esta completamente libre.\n";
        return;  // No hay bloques de memoria asignados
    }

    // Recorrer la pila de memoria y mostrar los bloques asignados
    BloqueMemoria* actual = topeMemoria;
    while (actual != NULL) {
        // Mostrar informaci�n del proceso y el tama�o del bloque de memoria
        cout << "Proceso " << actual->proceso->pid
             << " - Nombre: " << actual->proceso->nombre
             << " - Tama�o: " << actual->tamanio << "\n";
        actual = actual->siguiente;  // Mover al siguiente bloque en la pila
    }
}

//---------------- MEN� PRINCIPAL ----------------//

void menu() {
    int opcionPrincipal;

    do {
        do {
            cout << "\n=== Menu Principal ===\n";
            cout << "1. Gestor de Procesos\n";
            cout << "2. Planificador de CPU (Cola de Prioridad)\n";
            cout << "3. Gestor de Memoria\n";
            cout << "4. Salir\n";
            cout << "Seleccione una opcion (1-4): ";
            cin >> opcionPrincipal;

            if (opcionPrincipal < 1 || opcionPrincipal > 4) {
                cout << "Opcion invalida. Por favor ingrese un n�mero entre 1 y 4.\n";
            }
            cin.clear();
        } while (opcionPrincipal < 1 || opcionPrincipal > 4);

        switch (opcionPrincipal) {
            case 1: {
                int opcionProceso;
                do {
                    cout << "\n--- Gestor de Procesos ---\n";
                    cout << "1. Insertar nuevo proceso\n";
                    cout << "2. Eliminar proceso\n";
                    cout << "3. Mostrar todos los procesos\n";
                    cout << "4. Volver al menu principal\n";
                    cout << "Seleccione una opcion (1-4): ";
                    cin >> opcionProceso;

                    switch (opcionProceso) {
                        case 1: insertarProceso(); break;
                        case 2: eliminarProceso(); break;
                        case 3: mostrarProcesos(); break;
                        case 4: cout << "Volviendo al menu principal...\n"; break;
                        default: cout << "Opcion invalida.\n"; break;
                    }

                    system("pause"); // Espera una tecla
                    system("cls");   // Limpia pantalla
                } while (opcionProceso != 4);
                break;
            }
            case 2: {
                int opcionPlanificador;
                do {
                    cout << "\n=== Planificador de CPU (Cola de Prioridad) ===\n";
                    cout << "1. Encolar proceso por PID\n";
                    cout << "2. Desencolar y ejecutar proceso\n";
                    cout << "3. Mostrar cola actual\n";
                    cout << "4. Volver al menu principal\n";
                    cout << "Seleccione una opcion (1-4): ";
                    cin >> opcionPlanificador;

                    switch (opcionPlanificador) {
                        case 1: encolarProcesoEnPlanificador(); break;
                        case 2: desencolarYEjecutarProceso(); break;
                        case 3: mostrarColaPlanificador(); break;
                        case 4: cout << "Volviendo al menu principal...\n"; break;
                        default: cout << "Opcion invalida.\n"; break;
                    }

                    system("pause");
                    system("cls");
                } while (opcionPlanificador != 4);
                break;
            }
            case 3: {
                int opcionMemoria;
                do {
                    cout << "\n--- Gestor de Memoria ---\n";
                    cout << "1. Asignar memoria a proceso (push)\n";
                    cout << "2. Liberar memoria (pop)\n";
                    cout << "3. Ver estado actual de la memoria\n";
                    cout << "4. Volver al menu principal\n";
                    cout << "Seleccione una opcion (1-4): ";
                    cin >> opcionMemoria;

                    switch (opcionMemoria) {
                        case 1: asignarMemoria(); break;
                        case 2: liberarMemoria(); break;
                        case 3: estadoMemoria(); break;
                        case 4: cout << "Volviendo al menu principal...\n"; break;
                        default: cout << "Opcion invalida.\n"; break;
                    }

                    system("pause");
                    system("cls");
                } while (opcionMemoria != 4);
                break;
            }
            case 4:
                cout << "Saliendo del programa...\n";
                break;
        }

    } while (opcionPrincipal != 4);
}



// Funci�n principal del programa
int main() {
    menu(); // Llama al men� principal
    return 0;
}
