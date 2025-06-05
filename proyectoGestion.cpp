#include <iostream>
#include <string>

using namespace std;

struct Proceso {
    int pid;
    string nombre;
    int prioridad;
    Proceso* siguiente;
};

Proceso* cabezaProcesos = NULL;

struct BloqueMemoria {
    int pid;
    int tamanio;
    BloqueMemoria* siguiente;
};

BloqueMemoria* topeMemoria = NULL;

// --- Funciones Gestor de Procesos ---

// Inserta un nuevo proceso, pidiendo datos al usuario directamente
void insertarProceso() {
    int pid, prioridad;
    string nombre;

    cout << "Ingrese PID: ";
    cin >> pid;
    cin.ignore();
    cout << "Ingrese nombre: ";
    getline(cin, nombre);
    cout << "Ingrese prioridad: ";
    cin >> prioridad;

    // Lógica original para insertar proceso
    Proceso* nuevo = new Proceso;
    nuevo->pid = pid;
    nuevo->nombre = nombre;
    nuevo->prioridad = prioridad;
    nuevo->siguiente = NULL;

    if (cabezaProcesos == NULL) {
        cabezaProcesos = nuevo;
    } else {
        Proceso* actual = cabezaProcesos;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
    cout << "Proceso insertado correctamente.\n";
}


// Elimina un proceso pidiendo PID directamente
void eliminarProceso() {
    int pid;
    cout << "Ingrese PID a eliminar: ";
    cin >> pid;

    if (cabezaProcesos == NULL) {
        cout << "No hay procesos para eliminar.\n";
        return;
    }
    if (cabezaProcesos->pid == pid) {
        Proceso* temp = cabezaProcesos;
        cabezaProcesos = cabezaProcesos->siguiente;
        delete temp;
        cout << "Proceso eliminado.\n";
        return;
    }
    Proceso* actual = cabezaProcesos;
    while (actual->siguiente != NULL && actual->siguiente->pid != pid) {
        actual = actual->siguiente;
    }
    if (actual->siguiente != NULL) {
        Proceso* temp = actual->siguiente;
        actual->siguiente = temp->siguiente;
        delete temp;
        cout << "Proceso eliminado.\n";
    } else {
        cout << "Proceso no encontrado.\n";
    }
}

// Busca proceso por ID, pide ID al usuario
void buscarPorID() {
    int pid;
    cout << "Ingrese PID a buscar: ";
    cin >> pid;

    Proceso* actual = cabezaProcesos;
    while (actual != NULL && actual->pid != pid) actual = actual->siguiente;

    if (actual != NULL)
        cout << "Encontrado: PID=" << actual->pid << ", Nombre: " << actual->nombre << ", Prioridad: " << actual->prioridad << endl;
    else
        cout << "Proceso no encontrado.\n";
}

// Busca proceso por nombre, pide nombre al usuario
void buscarPorNombre() {
    string nombre;
    cout << "Ingrese nombre a buscar: ";
    cin.ignore();
    getline(cin, nombre);

    Proceso* actual = cabezaProcesos;
    while (actual != NULL && actual->nombre != nombre) actual = actual->siguiente;

    if (actual != NULL)
        cout << "Encontrado: PID=" << actual->pid << ", Nombre: " << actual->nombre << ", Prioridad: " << actual->prioridad << endl;
    else
        cout << "Proceso no encontrado.\n";
}

// Modifica la prioridad pidiendo datos al usuario
void modificarPrioridad() {
    int pid, prioridad;
    cout << "Ingrese PID a modificar: ";
    cin >> pid;
    cout << "Ingrese nueva prioridad: ";
    cin >> prioridad;

    Proceso* p = cabezaProcesos;
    while (p != NULL && p->pid != pid) p = p->siguiente;

    if (p != NULL) {
        p->prioridad = prioridad;
        cout << "Prioridad modificada.\n";
    } else {
        cout << "Proceso no encontrado.\n";
    }
}
// Muestra todos los procesos
void mostrarProcesos() {
    if (cabezaProcesos == NULL) {
        cout << "No hay procesos registrados." << endl;
        return;
    }
    Proceso* actual = cabezaProcesos;
    while (actual != NULL) {
        cout << "PID: " << actual->pid << ", Nombre: " << actual->nombre << ", Prioridad: " << actual->prioridad << endl;
        actual = actual->siguiente;
    }
}

// Asigna memoria pidiendo PID y tamaño
void asignarMemoria() {
    int pid, tamanio;
    cout << "Ingrese PID del proceso: ";
    cin >> pid;
    cout << "Ingrese tamanio de memoria a asignar: ";
    cin >> tamanio;

    BloqueMemoria* nuevo = new BloqueMemoria;
    nuevo->pid = pid;
    nuevo->tamanio = tamanio;
    nuevo->siguiente = topeMemoria;
    topeMemoria = nuevo;

    cout << "Memoria asignada al proceso " << pid << " (tamanio: " << tamanio << ")" << endl;
}

// Libera memoria del tope
void liberarMemoria() {
    if (topeMemoria == NULL) {
        cout << "No hay memoria para liberar." << endl;
        return;
    }
    BloqueMemoria* temp = topeMemoria;
    topeMemoria = topeMemoria->siguiente;
    cout << "Memoria liberada del proceso " << temp->pid << " (tamanio: " << temp->tamanio << ")" << endl;
    delete temp;
}

// Muestra el estado de la memoria
void estadoMemoria() {
    if (topeMemoria == NULL) {
        cout << "La memoria está completamente libre." << endl;
        return;
    }
    cout << "Estado actual de la memoria (tope a base):" << endl;
    BloqueMemoria* actual = topeMemoria;
    while (actual != NULL) {
        cout << "Proceso " << actual->pid << " - Tamanio: " << actual->tamanio << endl;
        actual = actual->siguiente;
    }
}

// --- Menu principal ---

void menu() {
    int opcionPrincipal = 0;

    do {
        system("cls");
        cout << "=== Menu Principal ===\n";
        cout << "1. Gestor de Procesos\n";
        cout << "2. Gestor de Memoria\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcionPrincipal;
        cin.ignore();

        switch (opcionPrincipal) {
            case 1: {
                int opProceso = 0;
                do {
                    system("cls");
                    cout << "--- Gestor de Procesos ---\n";
                    cout << "1. Insertar nuevo proceso\n";
                    cout << "2. Eliminar proceso\n";
                    cout << "3. Buscar proceso por ID\n";
                    cout << "4. Buscar proceso por nombre\n";
                    cout << "5. Modificar prioridad de proceso\n";
                    cout << "6. Mostrar todos los procesos\n";
                    cout << "7. Volver al menu principal\n";
                    cout << "Seleccione una opcion: ";
                    cin >> opProceso;
                    cin.ignore();

                    switch(opProceso) {
						case 1: insertarProceso(); break;
						case 2: eliminarProceso(); break;
						case 3: buscarPorID(); break;
						case 4: buscarPorNombre(); break;
						case 5: modificarPrioridad(); break;
						case 6: mostrarProcesos(); break;
                        case 7: cout << "Volviendo al menu principal...\n"; break;
                        default: cout << "Opcion invalida.\n"; break;
                    }
                    system("pause");
                } while (opProceso != 7);
                break;
            }
            case 2: {
                int opMemoria = 0;
                do {
                    system("cls");
                    cout << "--- Gestor de Memoria ---\n";
                    cout << "1. Asignar memoria a proceso (push)\n";
                    cout << "2. Liberar memoria (pop)\n";
                    cout << "3. Ver estado actual de la memoria\n";
                    cout << "4. Volver al menu principal\n";
                    cout << "Seleccione una opcion: ";
                    cin >> opMemoria;
                    cin.ignore();

                    switch(opMemoria) {
						case 1: asignarMemoria(); break;
						case 2: liberarMemoria(); break;
						case 3: estadoMemoria(); break;
                        case 4: cout << "Volviendo al menu principal...\n"; break;
                        default: cout << "Opcion invalida.\n"; break;
                    }
                    system("pause");
                } while (opMemoria != 4);
                break;
            }
            case 3:
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
                system("pause");
                break;
        }
    } while (opcionPrincipal != 3);
}

int main() {
    menu();
    return 0;
}
