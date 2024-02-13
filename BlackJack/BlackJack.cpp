#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

const int TAMANIO_BARAJA = 52;
const int TAMANIO_MANO = 11;

enum TipoCarta { CORAZONES, PICAS, TREBOLES, DIAMANTES };

void barajear(int baraja[], TipoCarta tipos[]) {
    for (int i = 0; i < TAMANIO_BARAJA; ++i) {
        baraja[i] = (i % 13) + 1;
        if (baraja[i] > 10) {
            baraja[i] = 10;
        }
        tipos[i] = static_cast<TipoCarta>(i / 13);
    }

    for (int i = 0; i < TAMANIO_BARAJA; ++i) {
        int carta1 = rand() % TAMANIO_BARAJA;
        int carta2 = rand() % TAMANIO_BARAJA;
        swap(baraja[carta1], baraja[carta2]);
        swap(tipos[carta1], tipos[carta2]);
    }
}

int calcularPuntuacion(int mano[], int numCartas) {
    int puntuacion = 0;
    int ases = 0;

    for (int i = 0; i < numCartas; ++i) {
        int valor = mano[i];
        if (valor == 1) {
            ases++;
            puntuacion += 11;
        }
        else {
            puntuacion += valor;
        }
    }

    while (puntuacion > 21 && ases > 0) {
        puntuacion -= 10;
        ases--;
    }

    return puntuacion;
}

string nombreTipoCarta(TipoCarta tipo) {
    switch (tipo) {
    case CORAZONES:
        return "Corazones";
    case PICAS:
        return "Picas";
    case TREBOLES:
        return "Treboles";
    case DIAMANTES:
        return "Diamantes";
    default:
        return "Desconocido";
    }
}

int valorAsJugador() {
    int valorAs;
    cout << "Que valor desea para el As? (1/11): ";
    cin >> valorAs;
    while (valorAs != 1 && valorAs != 11) {
        cout << "Valor invalido. Introduce 1 o 11: ";
        cin >> valorAs;
    }
    return valorAs;
}

int valorAsCroupier(int mano[], int numCartas) {
    int puntuacion = calcularPuntuacion(mano, numCartas);
    if (puntuacion + 11 > 21) {
        return 1;
    }
    else {
        return 11;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int baraja[TAMANIO_BARAJA];
    TipoCarta tipos[TAMANIO_BARAJA];

    barajear(baraja, tipos);

    int mano[TAMANIO_MANO] = { 0 };
    int manoBanca[TAMANIO_MANO] = { 0 };
    int cartaActual = 0;
    int cartaExtra = 0;
    int cartaExtraBanca = 0;

    for (int i = 0; i < 2; ++i) {
        mano[i] = baraja[cartaActual++];
    }

    manoBanca[0] = baraja[cartaActual++];
    cout << "La carta del croupier es: " << manoBanca[0] << " de " << nombreTipoCarta(tipos[0]) << endl;

    cout << "Las cartas del jugador son: " << endl;
    for (int i = 0; i < 2; ++i) {
        if (mano[i] == 1) {
            mano[i] = valorAsJugador();
        }
        cout << "\t" << mano[i] << " de " << nombreTipoCarta(tipos[i]) << endl;
    }

    char respuesta = 's';

    while (respuesta == 's') {
        int puntuacionJugador = calcularPuntuacion(mano, cartaExtra + 2);
        if (puntuacionJugador > 21) {
            cout << "El jugador se paso de 21. Gana la banca" << endl;
            break;
        }

        cout << "Desea otra carta? (s/n): ";
        cin >> respuesta;

        if (respuesta == 's') {
            if (cartaActual < TAMANIO_BARAJA) {
                mano[cartaExtra + 2] = baraja[cartaActual++];
                cartaExtra++;

                cout << "Las cartas del jugador son: " << endl;
                for (int i = 0; i < cartaExtra + 2; ++i) {
                    if (mano[i] == 1) {
                        mano[i] = valorAsJugador();
                    }
                    cout << "\t" << mano[i] << " de " << nombreTipoCarta(tipos[i]) << endl;
                }
            }
            else {
                cout << "No quedan cartas en la baraja." << endl;
                break;
            }
        }
    }

    if (calcularPuntuacion(mano, cartaExtra + 2) <= 21) {
        int valorAs = valorAsCroupier(manoBanca, cartaExtraBanca + 2);
        while (calcularPuntuacion(manoBanca, cartaExtraBanca + 2) < calcularPuntuacion(mano, cartaExtra + 2) && calcularPuntuacion(manoBanca, cartaExtraBanca + 2) <= 21) {
            if (cartaActual < TAMANIO_BARAJA) {
                manoBanca[cartaExtraBanca + 2] = baraja[cartaActual++];
                cartaExtraBanca++;
            }
            else {
                cout << "No quedan cartas en la baraja." << endl;
                break;
            }
        }

        cout << "Las cartas del croupier son: " << endl;
        for (int i = 0; i < cartaExtraBanca + 1; ++i) {
            cout << "\t" << manoBanca[i] << " de " << nombreTipoCarta(tipos[cartaExtra + i]) << endl;
        }


        int puntuacionJugador = calcularPuntuacion(mano, cartaExtra + 2);
        int puntuacionBanca = calcularPuntuacion(manoBanca, cartaExtraBanca + 2);

        cout << "Puntuacion del jugador: " << puntuacionJugador << endl;
        cout << "Puntuacion de la banca: " << puntuacionBanca << endl;

        if ((puntuacionJugador == 21 && puntuacionBanca != 21) || (puntuacionJugador < 21 && puntuacionBanca > 21)) {
            cout << "El jugador gana y la banca pierde." << endl;
        }
        else if ((puntuacionJugador <= 21 && puntuacionBanca <= 21) && puntuacionJugador == puntuacionBanca) {
            cout << "Empate. El jugador y la banca tienen la misma puntuacion." << endl;
        }
        else {
            cout << "El jugador pierde y la banca gana." << endl;
        }
    }

    return 0;
}
