#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

// Clases y estructuras
struct Objeto {
    string nombre;
    int bonificacionAtaque;
    int bonificacionDefensa;
};

struct Enemigo {
    string nombre;
    int vida;
    int ataque;
    int nivel;
};

class Jugador {
public:
    string nombre;
    int vida;
    int ataque;
    int defensa;
    int nivel;
    int experiencia;
    vector<Objeto> inventario;
    bool habilidadDisponible;

    Jugador(string nombre) : nombre(nombre), vida(100), ataque(10), defensa(5), nivel(1), experiencia(0), habilidadDisponible(true) {}

    void mostrarEstadisticas() {
        cout << "\n--- Estadisticas de " << nombre << " ---\n";
        cout << "Nivel: " << nivel << "\n";
        cout << "Vida: " << vida << "\n";
        cout << "Ataque: " << ataque << "\n";
        cout << "Defensa: " << defensa << "\n";
        cout << "Experiencia: " << experiencia << "/" << nivel * 50 << "\n";
        cout << "--------------------------------\n";
    }

    void recogerObjeto(Objeto objeto) {
        inventario.push_back(objeto);
        ataque += objeto.bonificacionAtaque;
        defensa += objeto.bonificacionDefensa;
        cout << "Has recogido un " << objeto.nombre << "! Ataque y defensa aumentados.\n";
    }

    void mostrarInventario() {
        cout << "\n--- Inventario ---\n";
        if (inventario.empty()) {
            cout << "Tu inventario esta vacio.\n";
        } else {
            for (const auto& obj : inventario) {
                cout << obj.nombre << " (Ataque: +" << obj.bonificacionAtaque << ", Defensa: +" << obj.bonificacionDefensa << ")\n";
            }
        }
        cout << "------------------\n";
    }

    void ganarExperiencia(int exp) {
        experiencia += exp;
        if (experiencia >= nivel * 50) {  // Requiere 50 puntos * nivel actual para subir de nivel
            experiencia = 0;
            nivel++;
            ataque += 5;
            defensa += 3;
            vida += 20;
            cout << "¡Subiste al nivel " << nivel << "! Vida, ataque y defensa mejorados.\n";
        }
    }
};

// Declaración de funciones
void mostrarMenu();
void instrucciones();
void explorarBosque(Jugador &jugador);
void luchar(Jugador &jugador, Enemigo enemigo);
bool batalla(Jugador &jugador, Enemigo &enemigo);
void usarHabilidad(Jugador &jugador, Enemigo &enemigo);
void eventoAleatorio(Jugador &jugador);
void reiniciarJuego(Jugador &jugador);

// Función principal
int main() {
    srand(static_cast<unsigned int>(time(0))); // Semilla para la aleatoriedad

    cout << "Bienvenido a 'Aventura en el Bosque'!\n";
    cout << "Ingresa el nombre de tu personaje: ";
    string nombreJugador;
    getline(cin, nombreJugador);

    Jugador jugador(nombreJugador);
    int opcion;

    do {
        mostrarMenu();
        cout << "Elige una opcion: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(opcion) {
            case 1:
                instrucciones();
                break;
            case 2:
                explorarBosque(jugador);
                break;
            case 3:
                jugador.mostrarEstadisticas();
                break;
            case 4:
                jugador.mostrarInventario();
                break;
            case 5:
                cout << "Saliendo del juego...\n";
                break;
            default:
                cout << "Opcion invalida. Intenta nuevamente.\n";
        }
    } while(opcion != 5);

    return 0;
}

// Mostrar el menú principal
void mostrarMenu() {
    cout << "\n--- Menú Principal ---\n";
    cout << "1. Instrucciones\n";
    cout << "2. Explorar el Bosque\n";
    cout << "3. Mostrar Estadisticas\n";
    cout << "4. Mostrar Inventario\n";
    cout << "5. Salir\n";
}

// Instrucciones del juego
void instrucciones() {
    cout << "\n--- Instrucciones ---\n";
    cout << "Explora el bosque y sube de nivel encontrando objetos y luchando contra enemigos.\n";
    cout << "Recoge objetos para aumentar tu ataque y defensa, y usa tu habilidad especial en combate.\n";
    cout << "Cada vez que exploras, puedes encontrar sorpresas, enemigos o trampas.\n";
    cout << "Objetivo: Sobrevive el mayor tiempo posible. ¡Buena suerte!\n";
}

// Explorar el bosque con eventos aleatorios
void explorarBosque(Jugador &jugador) {
    int evento = rand() % 4; // 0: objeto, 1: enemigo, 2: evento especial, 3: trampa

    switch (evento) {
        case 0: {  // Encuentra un objeto
            vector<Objeto> objetosDisponibles = {
                {"Espada Antigua", 5, 0},
                {"Escudo Roto", 0, 3},
                {"Armadura de Cuero", 0, 4},
                {"Anillo de Poder", 3, 3}
            };
            Objeto objetoEncontrado = objetosDisponibles[rand() % objetosDisponibles.size()];
            cout << "Has encontrado un " << objetoEncontrado.nombre << "!\n";
            jugador.recogerObjeto(objetoEncontrado);
            break;
        }
        case 1: {  // Encuentra un enemigo
            vector<Enemigo> enemigosDisponibles = {
                {"Lobo Salvaje", 30 + jugador.nivel * 5, 8 + jugador.nivel * 2, jugador.nivel},
                {"Bandido", 40 + jugador.nivel * 6, 10 + jugador.nivel * 2, jugador.nivel},
                {"Troll", 50 + jugador.nivel * 7, 12 + jugador.nivel * 2, jugador.nivel},
                {"Dragon Pequenio", 70 + jugador.nivel * 10, 15 + jugador.nivel * 3, jugador.nivel}
            };
            Enemigo enemigoEncontrado = enemigosDisponibles[rand() % enemigosDisponibles.size()];
            cout << "Te has encontrado con un " << enemigoEncontrado.nombre << "!\n";
            luchar(jugador, enemigoEncontrado);
            break;
        }
        case 2:  // Evento especial
            eventoAleatorio(jugador);
            break;
        case 3:  // Trampa
            int danoTrampa = 10 + rand() % 15;
            cout << "¡Oh no! Has caido en una trampa y has recibido " << danoTrampa << " de danio.\n";
            jugador.vida -= danoTrampa;
            if (jugador.vida <= 0) {
                cout << "Has sido derrotado. Juego Terminado.\n";
                reiniciarJuego(jugador);
            }
            break;
    }
}

// Evento especial aleatorio
void eventoAleatorio(Jugador &jugador) {
    int evento = rand() % 2;
    if (evento == 0) {
        cout << "Te encuentras con un sabio que te ofrece curarte.\n";
        int curacion = 20 + rand() % 20;
        jugador.vida = min(jugador.vida + curacion, 100);
        cout << "Has sido curado " << curacion << " puntos de vida.\n";
    } else {
        cout << "Encuentras un lago encantado que aumenta tu defensa temporalmente.\n";
        jugador.defensa += 3;
        cout << "Defensa aumentada en +3!\n";
    }
}

// Luchar contra un enemigo
void luchar(Jugador &jugador, Enemigo enemigo) {
    cout << "Entras en combate con el " << enemigo.nombre << " (Nivel " << enemigo.nivel << ").\n";
    bool victoria = batalla(jugador, enemigo);
    if (victoria) {
        int expGanada = enemigo.nivel * 20;
        cout << "Has derrotado al " << enemigo.nombre << " y ganas " << expGanada << " puntos de experiencia.\n";
        jugador.ganarExperiencia(expGanada);
    } else {
        cout << "Has sido derrotado por " << enemigo.nombre << ".\n";
        reiniciarJuego(jugador);
    }
}

// Función que simula la batalla
bool batalla(Jugador &jugador, Enemigo &enemigo) {
    while (jugador.vida > 0 && enemigo.vida > 0) {
        cout << jugador.nombre << " (Vida: " << jugador.vida << ") vs " << enemigo.nombre << " (Vida: " << enemigo.vida << ")\n";

        if (jugador.habilidadDisponible) {
            cout << "¿Quieres usar tu habilidad especial? (s/n): ";
            char opcion;
            cin >> opcion;
            if (opcion == 's' || opcion == 'S') {
                usarHabilidad(jugador, enemigo);
                jugador.habilidadDisponible = false;
            }
        }

        int danoJugador = jugador.ataque - enemigo.nivel;
        int danoEnemigo = enemigo.ataque - jugador.defensa;

        enemigo.vida -= danoJugador;
        jugador.vida -= danoEnemigo;

        cout << jugador.nombre << " inflige " << danoJugador << " de danio al " << enemigo.nombre << ".\n";
        if (enemigo.vida > 0) {
            cout << enemigo.nombre << " inflige " << danoEnemigo << " de danio a " << jugador.nombre << ".\n";
        }
    }
    return jugador.vida > 0;
}

// Usar habilidad especial
void usarHabilidad(Jugador &jugador, Enemigo &enemigo) {
    int danoEspecial = jugador.ataque * 2;
    enemigo.vida -= danoEspecial;
    cout << jugador.nombre << " usa su habilidad especial y hace " << danoEspecial << " de danio al " << enemigo.nombre << "!\n";
}

// Reiniciar el juego
void reiniciarJuego(Jugador &jugador) {
    cout << "¿Quieres intentarlo de nuevo? (s/n): ";
    char opcion;
    cin >> opcion;
    if (opcion == 's' || opcion == 'S') {
        jugador.vida = 100;
        jugador.ataque = 10;
        jugador.defensa = 5;
        jugador.nivel = 1;
        jugador.experiencia = 0;
        jugador.habilidadDisponible = true;
        jugador.inventario.clear();
        cout << "Has reiniciado el juego. ¡Buena suerte!\n";
    } else {
        cout << "Gracias por jugar. Hasta la proxima.\n";
        exit(0);
    }
}
