#include <iostream>
#include <cstdlib>
#include <string.h>
#include <cstring>
#include <cstdio>
#include <time.h> 
#include <iomanip>
#include <windows.h>
#include <vector>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

struct Armamento{ // Almacena datos del arma equipada
	string Tipo = "?"; // hacha, espada, arco, daga, desarmado
	string Nombre;
	int Nvl = 1;
	int DMGF = 0; // daño fisico
	int DMGV = 0; // daño veneno
	int DMGM = 0; // daño magico
	int DMGFG = 0; // daño fuego
	bool enUso = false;
};

struct Armadura{
	string Nombre = "?";
	int Nvl = 1;
	int RESF = 0;
	int RESV = 0;
	int RESM = 0;
	int RESFG = 0;
	bool enUso = false;
};

struct Estadisticas{
	int Fuerza = 0;
	int Resistencia = 0;
	int RegFisica = 0;
	int Vitalidad = 0;
};

struct Datos{ // Almacena datos del jugador
	string Nombre;
	int Clase;
	int Nivel = 1;
	int Salud;
	int SaludMax;
	int CantXP = 0;
	int CantXPMaxima = 50;
	int Oro = 50;
	int InvUsado = 1;
	int puntosDisp = 0;
	int muertes = 0;
	Estadisticas Caracteristicas;
	Armamento ArmaEquipada;
	Armadura ArmaduraEquipada;
};

struct Item{ // Almacena datos de items para el inv (puestos en un vector)
	int id = 0;
	int nivel = 1;
	string Nombre;
	Armamento Datos;
	Armadura DatosB;
};

struct Enemigo{
	string Nombre;
	int Nivel;
	int Salud;
	int SaludMax;
	int XPDrop;
	Armamento ArmaEquipada;
	Armadura ArmaduraEquipada;
};

Datos Jugador;
vector<Item> Inventario;
Item ItemTMP;

int Dificultad;
int Opcion;
int regeneracionFisica = 0;
int saludMas = 0;
int seed = 1;
float modificadorAtaque = 0.00;
float modificadorResistencia = 0.00;
string texto;
string texto2;
string region;
bool mentorActivo = false;
bool turnoActivo = false;
bool turnoEnemigo = false;
bool jugadorEnvenenado = false;
bool enemigoEnvenenado = false;
bool enCombate = false;
bool cofreActivo = false;

Enemigo Enemigo;

// variables del mercader
bool mercaderActivo = false;
int mercaderId = 0;
int mercaderPrecio = 0;
string mercaderNombre;


void darPaso();
void inv();
void stats();
void mostStats();
void interactuar();
void combate();
void mostCombate();
void obtenerLoot();

void jugar(){
	SetConsoleTextAttribute(hConsole, 15);
	enCombate = false;
	texto2 = "";
	stats();
	system("cls");
	cout << "--        " << region << "        --\n";
	cout << "\n" << texto << "\n\n";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "(1) ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << " Dar un paso  ";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "(2) ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << " Inventario  ";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "(3) "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << " Interactuar  ";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "(4) ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << " Caracteristicas" << "\n"; 
	cout << "\n";
	cout << "|-        " << Jugador.Nombre << "        -|";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "\n  Nivel "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.Nivel;
	SetConsoleTextAttribute(hConsole, 3);
	cout << "\n  XP ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.CantXP;
	SetConsoleTextAttribute(hConsole, 3);
	cout << " / ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.CantXPMaxima;
	SetConsoleTextAttribute(hConsole, 12);
	cout << "\n  Salud "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.Salud;
	SetConsoleTextAttribute(hConsole, 12);
	cout << " / "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.SaludMax; 
	if (mentorActivo == true){
		SetConsoleTextAttribute(hConsole, 3);
		cout << "\n  Pocion del mentor activa";
	}
	SetConsoleTextAttribute(hConsole, 6);
	cout << "\n\n  Oro "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.Oro;
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\n\nElige la opcion que vas a elegir: ";
	SetConsoleTextAttribute(hConsole, 7);
	cin >> Opcion;
	if (Opcion == 1){
		darPaso();
	}
	else if (Opcion == 2){
		inv();
	}
	else if (Opcion == 3){
		interactuar();
	}
	else if (Opcion == 4){
		mostStats();
	}
	else{
		texto = "La opcion no es valida.";
		jugar();
	}
}

string agregarArmadura(int id, int nivel, string nombre, Armadura datos){
	// Armadura Pechera;
	// Pechera.RESF = 132;
	// Pechera.Nombre = "Armadura del Astolfo";
	// Pechera.Nvl = 1;
 	// agregarArmadura(4, Pechera.Nvl, "Armadura del Astolfo", Pechera);
	for (int i=0;i<Inventario.size();i++){
		if (Inventario[i].id == 0){
			ItemTMP.id = id;
			ItemTMP.nivel = nivel;
			ItemTMP.Nombre = nombre;
			ItemTMP.DatosB = datos;
			Inventario.push_back(ItemTMP);
			break;
		}
	}
	return "OK.";
}

string agregarArma(int id, int nivel, string nombre, Armamento datos){
	// Armamento Daga;
	// Daga.DMGF = 152;
	// Daga.DMGFG = 647;
	// Daga.Nombre = "Daga del Astolfo";
	// Daga.Nvl = 1;
	// Daga.Tipo = "Daga";
 	// agregarArma(3, Daga.Nvl, "Daga del Astolfo", Daga);
	for (int i=0;i<Inventario.size();i++){
		if (Inventario[i].id == 0){
			ItemTMP.id = id;
			ItemTMP.nivel = nivel;
			ItemTMP.Nombre = nombre;
			ItemTMP.Datos = datos;
			Inventario.push_back(ItemTMP);
			break;
		}
	}
	return "OK.";
}

string agregarItem(int id, int nivel, string nombre){
	for (int i=0;i<Inventario.size();i++){
		if (Inventario[i].id == 0){
			ItemTMP.id = id;
			ItemTMP.nivel = nivel;
			ItemTMP.Nombre = nombre;
			Inventario.push_back(ItemTMP);
			break;
		}
	}
	return "OK.";
}

string usarItem(int id, int nivel, int Opcion){
	// Esto define la funcion de cada item 
	if (id == 1){ // Pocion de curacion
		if (Jugador.Salud < Jugador.SaludMax){
			int cnt = 40 + (15 * nivel);
			int cnt2;
			Jugador.Salud = Jugador.Salud + cnt;
			if (Jugador.Salud > Jugador.SaludMax){
				cnt2 = Jugador.Salud - Jugador.SaludMax;
				cnt = cnt - cnt2;
			}
			stats();
			Inventario[Opcion].id = 0;
			return "\nUsaste Pocion de Curacion.\nRegeneraste " + std::to_string(cnt) + " de salud (" + std::to_string(Jugador.Salud) + "/" + std::to_string(Jugador.SaludMax) + ")."; 
		}
		else{
			return "\nNo puedes usar esto si tu salud esta intacta.";
		}
	}
	else if (id == 2){ // Pocion del mentor
		if (mentorActivo == false){
			mentorActivo = true;
			Inventario[Opcion].id = 0;
			return "\nUsaste Pocion del Mentor.\nRecibiras el doble de XP en tu proximo combate."; 
		}
		else{
			return "\nNo puedes usar esto si ya tienes una pocion del mentor activa.";
		}
	}
	else if (id == 7){ // Pocion del turno
		if (turnoActivo == false){
			if (enCombate == true){
				turnoActivo = true;
				Inventario[Opcion].id = 0;
				return "\nUsaste Pocion del Turno.\nTendras doble turno en tu proximo combate."; 
			}
			else{
				return "\nNo puedes usar esto si no estas en combate.";
			}
		}
		else{
			return "\nNo puedes usar esto si ya tienes una pocion del turno activa.";
		}
	}
	return "Invalido.";
}

void obtenerLoot(){
	SetConsoleTextAttribute(hConsole, 15);
	cout << "--       Item obtenido        --\n";
	Item loot;
	agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	srand(time(0));
	int N = ((rand() % 39));
	if (N == 0){
		loot.id = 1;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Pocion de Curacion";
		agregarItem(loot.id, loot.nivel, loot.Nombre);
	}
	else if (N == 1){
		loot.id = 2;
		loot.nivel = Jugador.Nivel;
	   	loot.Nombre = "Pocion del Mentor";
	   	agregarItem(loot.id, loot.nivel, loot.Nombre);
	}
	else if (N == 2){
		loot.id = 3;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Daga comun";
		loot.Datos.DMGF = 8 * Jugador.Nivel;
		loot.Datos.DMGV = 3 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Daga";
		loot.Datos.Nombre = "Daga comun";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 3){
		loot.id = 3;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Daga del Novato";
		loot.Datos.DMGF = 10 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Daga";
		loot.Datos.Nombre = "Daga del Novato";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 4){
		loot.id = 3;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Daga oxidada";
		loot.Datos.DMGF = 7 * Jugador.Nivel;
		loot.Datos.DMGV = 3 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Daga";
		loot.Datos.Nombre = "Daga oxidada";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 5){
		loot.id = 4;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Espada del Inframundo";
		loot.Datos.DMGF = 13 * Jugador.Nivel;
		loot.Datos.DMGFG = 10 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Espada";
		loot.Datos.Nombre = "Espada del Inframundo";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 6){
		loot.id = 4;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Espada de Astolfo";
		loot.Datos.DMGF = 17 * Jugador.Nivel;
		loot.Datos.DMGFG = 3 * Jugador.Nivel;
		loot.Datos.DMGM = 6 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Espada";
		loot.Datos.Nombre = "Espada de Astolfo";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 7){
		loot.id = 4;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Espada de la Magia Oscura";
		loot.Datos.DMGF = 4 * Jugador.Nivel;
		loot.Datos.DMGM = 10 * Jugador.Nivel;
		loot.Datos.DMGV = 3 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Espada";
		loot.Datos.Nombre = "Espada de la Magia Oscura";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 8){
		loot.id = 4;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Espada del Angel";
		loot.Datos.DMGF = 14 * Jugador.Nivel;
		loot.Datos.DMGM = 4 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Espada";
		loot.Datos.Nombre = "Espada del Angel";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 9){
		loot.id = 4;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Espada de la Lealtad";
		loot.Datos.DMGF = 17 * Jugador.Nivel;
		loot.Datos.DMGFG = 2 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Espada";
		loot.Datos.Nombre = "Espada de la Lealtad";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 10){
		loot.id = 4;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Espada del plebeyo";
		loot.Datos.DMGF = 9 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Espada";
		loot.Datos.Nombre = "Espada del plebeyo";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 11){
		loot.id = 4;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Espada del Rey";
		loot.Datos.DMGF = 15 * Jugador.Nivel;
		loot.Datos.DMGFG = 7 * Jugador.Nivel;
		loot.Datos.DMGM = 4 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Espada";
		loot.Datos.Nombre = "Espada del Rey";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 12){
		loot.id = 5;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Hacha desgastada";
		loot.Datos.DMGF = 13 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Hacha";
		loot.Datos.Nombre = "Hacha desgastada";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 13){
		loot.id = 5;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Hacha del Guerrero";
		loot.Datos.DMGF = 17 * Jugador.Nivel;
		loot.Datos.DMGFG = 6 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Hacha";
		loot.Datos.Nombre = "Hacha del Guerrero";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 14){
		loot.id = 5;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Hacha del Novato";
		loot.Datos.DMGF = 11 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Hacha";
		loot.Datos.Nombre = "Hacha del Novato";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 15){
		loot.id = 5;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Hacha de la Sabiduria";
		loot.Datos.DMGF = 10 * Jugador.Nivel;
		loot.Datos.DMGM = 14 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Hacha";
		loot.Datos.Nombre = "Hacha de la Sabiduria";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 16){
		loot.id = 5;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Hacha venenosa";
		loot.Datos.DMGF = 15 * Jugador.Nivel;
		loot.Datos.DMGV = 8 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Hacha";
		loot.Datos.Nombre = "Hacha venenosa";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 17){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura del Ladron";
		loot.DatosB.RESF = 9 * Jugador.Nivel;
		loot.DatosB.RESV = 5 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura del Ladron";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 18){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura de la Realeza";
		loot.DatosB.RESF = 14 * Jugador.Nivel;
		loot.DatosB.RESV = 2 * Jugador.Nivel;
		loot.DatosB.RESFG = 5 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura de la Realeza";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 19){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura de Astolfo";
		loot.DatosB.RESF = 15 * Jugador.Nivel;
		loot.DatosB.RESV = 7 * Jugador.Nivel;
		loot.DatosB.RESFG = 4 * Jugador.Nivel;
		loot.DatosB.RESM = 6 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura de Astolfo";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 20){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura del Diablo";
		loot.DatosB.RESF = 15 * Jugador.Nivel;
		loot.DatosB.RESFG = 9 * Jugador.Nivel;
		loot.DatosB.RESM = 3 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura del Diablo";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 21){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura del Gato";
		loot.DatosB.RESF = 16 * Jugador.Nivel;
		loot.DatosB.RESV = 3 * Jugador.Nivel;
		loot.DatosB.RESM = 2 * Jugador.Nivel;
		loot.DatosB.RESFG = 5 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura del Gato";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 22){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura del Emperador";
		loot.DatosB.RESF = 17 * Jugador.Nivel;
		loot.DatosB.RESM = 4 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura del Emperador";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 23){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura celestial";
		loot.DatosB.RESF = 7 * Jugador.Nivel;
		loot.DatosB.RESM = 18 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura celestial";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 24){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura del Enfermo";
		loot.DatosB.RESF = 12 * Jugador.Nivel;
		loot.DatosB.RESV = 17 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura del Enfermo";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 25){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura del Rey";
		loot.DatosB.RESF = 16 * Jugador.Nivel;
		loot.DatosB.RESV = 6 * Jugador.Nivel;
		loot.DatosB.RESFG = 4 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura del Rey";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 26){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura balanceada";
		loot.DatosB.RESF = 15 * Jugador.Nivel;
		loot.DatosB.RESV = 8 * Jugador.Nivel;
		loot.DatosB.RESFG = 6 * Jugador.Nivel;
		loot.DatosB.RESM = 11 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura balanceada";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 27){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura de la Justicia";
		loot.DatosB.RESF = 20 * Jugador.Nivel;
		loot.DatosB.RESFG = 10 * Jugador.Nivel;
		loot.DatosB.RESV = 5 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura de la Justicia";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 28){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura de la Magia Oscura";
		loot.DatosB.RESF = 13 * Jugador.Nivel;
		loot.DatosB.RESM = 18 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura de la Magia Oscura";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 29){
		loot.id = 5;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Hacha del Rey";
		loot.Datos.DMGF = 17 * Jugador.Nivel;
		loot.Datos.DMGFG = 4 * Jugador.Nivel;
		loot.Datos.DMGV = 3 * Jugador.Nivel;
		loot.Datos.DMGM = 5 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Hacha";
		loot.Datos.Nombre = "Hacha del Rey";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 30){
		loot.id = 4;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Espada balanceada";
		loot.Datos.DMGF = 9 * Jugador.Nivel;
		loot.Datos.DMGM = 7 * Jugador.Nivel;
		loot.Datos.DMGFG = 5 * Jugador.Nivel;
		loot.Datos.DMGV = 4 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Espada";
		loot.Datos.Nombre = "Espada balanceada";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 31){
		loot.id = 4;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Espada de los Antiguos Esqueletos";
		loot.Datos.DMGF = 11 * Jugador.Nivel;
		loot.Datos.DMGM = 3 * Jugador.Nivel;
		loot.Datos.DMGV = 6 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Espada";
		loot.Datos.Nombre = "Espada de los Antiguos Esqueletos";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 32){
		loot.id = 3;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Daga balanceada";
		loot.Datos.DMGF = 8 * Jugador.Nivel;
		loot.Datos.DMGM = 4 * Jugador.Nivel;
		loot.Datos.DMGV = 3 * Jugador.Nivel;
		loot.Datos.DMGFG = 5 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Daga";
		loot.Datos.Nombre = "Daga balanceada";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 33){
		loot.id = 3;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Daga infernal";
		loot.Datos.DMGF = 9 * Jugador.Nivel;
		loot.Datos.DMGV = 2 * Jugador.Nivel;
		loot.Datos.DMGFG = 7 * Jugador.Nivel;
		loot.Datos.Nvl = Jugador.Nivel;
		loot.Datos.Tipo = "Daga";
		loot.Datos.Nombre = "Daga infernal";
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
	}
	else if (N == 34){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura infernal";
		loot.DatosB.RESF = 14 * Jugador.Nivel;
		loot.DatosB.RESFG = 17 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura infernal";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 35){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura de la Paz";
		loot.DatosB.RESF = 11 * Jugador.Nivel;
		loot.DatosB.RESM = 12 * Jugador.Nivel;
		loot.DatosB.RESFG = 8 * Jugador.Nivel;
		loot.DatosB.RESV = 6 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura de la Paz";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 36){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura de la Gracia";
		loot.DatosB.RESF = 12 * Jugador.Nivel;
		loot.DatosB.RESM = 11 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura de la Gracia";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 37){
		loot.id = 6;
		loot.nivel = Jugador.Nivel;
		loot.Nombre = "Armadura del Lobo";
		loot.DatosB.RESF = 17 * Jugador.Nivel;
		loot.DatosB.RESFG = 9 * Jugador.Nivel;
		loot.DatosB.Nvl = Jugador.Nivel;
		loot.DatosB.Nombre = "Armadura del Lobo";
		agregarArmadura(loot.id, loot.nivel, loot.Nombre, loot.DatosB);
	}
	else if (N == 38){
		loot.id = 7;
	   	loot.nivel = Jugador.Nivel;
	   	loot.Nombre = "Pocion del Turno";
	   	agregarItem(loot.id, loot.nivel, loot.Nombre);
	}
	int multiplicador1 = Jugador.ArmaEquipada.DMGF * modificadorAtaque;
	int multiplicador2 = Jugador.ArmaEquipada.DMGFG * modificadorAtaque;
	int multiplicador3 = Jugador.ArmaEquipada.DMGV * modificadorAtaque;
	int multiplicador4 = Jugador.ArmaEquipada.DMGM * modificadorAtaque;
	int multiplicador5 = Jugador.ArmaduraEquipada.RESF * modificadorResistencia;
	int multiplicador6 = Jugador.ArmaduraEquipada.RESFG * modificadorResistencia;
	int multiplicador7 = Jugador.ArmaduraEquipada.RESV * modificadorResistencia;
	int multiplicador8 = Jugador.ArmaduraEquipada.RESM * modificadorResistencia;
	if (loot.id == 3 || loot.id == 4 || loot.id == 5){
		SetConsoleTextAttribute(hConsole, 15);
		cout << loot.Nombre;
		SetConsoleTextAttribute(hConsole, 14);
		cout << " | "; 
		SetConsoleTextAttribute(hConsole, 15);
		cout << "Nivel ";
		cout << loot.nivel;
		SetConsoleTextAttribute(hConsole, 14);
		cout << "\nAtaque: Fisico "; 
		SetConsoleTextAttribute(hConsole, 15);
		cout << loot.Datos.DMGF + multiplicador1 << " -"; 
		SetConsoleTextAttribute(hConsole, 12);
		cout << " Fuego ";
		SetConsoleTextAttribute(hConsole, 15);
		cout << loot.Datos.DMGFG + multiplicador2 << " -";
		SetConsoleTextAttribute(hConsole, 10);
		cout << " Veneno "; 
		SetConsoleTextAttribute(hConsole, 15);
		cout << loot.Datos.DMGV + multiplicador3 << " -";
		SetConsoleTextAttribute(hConsole, 9);
		cout << " Magico ";
		SetConsoleTextAttribute(hConsole, 15);
		cout << loot.Datos.DMGM + multiplicador4;
	}
	texto = texto + "\nObtuviste " + loot.Nombre + " y ahora esta en tu inventario.";
	jugar();
}

string equiparArma(int Opcion){
	if (Jugador.Nivel >= Inventario[Opcion].Datos.Nvl){
		Jugador.ArmaEquipada = Inventario[Opcion].Datos;
		Inventario[Opcion].Datos.enUso = true;
		return "Equipaste " + Inventario[Opcion].Datos.Nombre + " como tu arma principal.";
	}
	else{
		return "No tienes el suficiente nivel como para usar esto.";
	}
}

string equiparArmadura(int Opcion){
	if (Jugador.Nivel >= Inventario[Opcion].DatosB.Nvl){
		Jugador.ArmaduraEquipada = Inventario[Opcion].DatosB;
		Inventario[Opcion].DatosB.enUso = true;
		return "Equipaste " + Inventario[Opcion].DatosB.Nombre + " como tu armadura.";
	}
	else{
		return "No tienes el suficiente nivel como para usar esto.";
	}
}

void interactuar(){
	if (mercaderActivo == true){
		if (Jugador.Oro >= mercaderPrecio && Jugador.InvUsado < 20){
			Jugador.Oro = Jugador.Oro - mercaderPrecio;
			agregarItem(mercaderId, Jugador.Nivel, mercaderNombre);
			texto = "Compraste " + mercaderNombre + " por " + std::to_string(mercaderPrecio) + " de oro.";
			mercaderActivo = false;
		}
		else{
			if (Jugador.Oro >= mercaderPrecio){
				texto = "No hay espacio en tu inventario. El mercader se ha ido.";
			}
			else{
				texto = "No tienes el suficiente oro para comprarlo. El mercader se ha ido.";
			}
			mercaderActivo = false;
		}
	}
	else{
		if (cofreActivo == true){
			cofreActivo = false;
			texto = "Abriste el cofre.";
			obtenerLoot();
		}
		else{
			texto = "En este momento no hay nada con lo que interactuar.";
		}
	}
	jugar();
}

void inv(){ // Inventario
	system("cls");
	SetConsoleTextAttribute(hConsole, 15);
	cout << "--        Inventario de " << Jugador.Nombre << "        --\n";
	for (int i=0;i<Inventario.size();i++){
		if (Inventario[i].id != 0){
			if (Inventario[i].Datos.Tipo != "?"){
				if (Inventario[i].Datos.enUso == false){
					SetConsoleTextAttribute(hConsole, 14);
					cout << "\n(" << i+1 << ") ";
					SetConsoleTextAttribute(hConsole, 15);
					cout << "- " << Inventario[i].Nombre << " (Nivel " << Inventario[i].nivel << ") (Ataque | Fisico " << Inventario[i].Datos.DMGF << " - Fuego " << Inventario[i].Datos.DMGFG << " - Veneno " << Inventario[i].Datos.DMGV << " - Magico " << Inventario[i].Datos.DMGM << ")"; 
				}
				else{
					SetConsoleTextAttribute(hConsole, 14);
					cout << "\n(" << i+1 << ") ";
					SetConsoleTextAttribute(hConsole, 15);
					cout << "- " << Inventario[i].Nombre << " (Nivel " << Inventario[i].nivel << ") (EN USO) (Ataque | Fisico " << Inventario[i].Datos.DMGF << " - Fuego " << Inventario[i].Datos.DMGFG << " - Veneno " << Inventario[i].Datos.DMGV << " - Magico " << Inventario[i].Datos.DMGM << ")"; 
				}
			}
			else{
				if (Inventario[i].DatosB.Nombre != "?"){
					if (Inventario[i].DatosB.enUso == false){
						SetConsoleTextAttribute(hConsole, 14);
						cout << "\n(" << i+1 << ") ";
						SetConsoleTextAttribute(hConsole, 15);
						cout << "- " << Inventario[i].Nombre << " (Nivel " << Inventario[i].nivel << ") (Resistencia | Fisico " << Inventario[i].DatosB.RESF << " - Fuego " << Inventario[i].DatosB.RESFG << " - Veneno " << Inventario[i].DatosB.RESV << " - Magico " << Inventario[i].DatosB.RESM << ")"; 
					}
					else{
						SetConsoleTextAttribute(hConsole, 14);
						cout << "\n(" << i+1 << ") ";
						SetConsoleTextAttribute(hConsole, 15);
						cout << "- " << Inventario[i].Nombre << " (Nivel " << Inventario[i].nivel << ") (EN USO) (Resistencia | Fisico " << Inventario[i].DatosB.RESF << " - Fuego " << Inventario[i].DatosB.RESFG << " - Veneno " << Inventario[i].DatosB.RESV << " - Magico " << Inventario[i].DatosB.RESM << ")"; 
					}
				}
				else{
					SetConsoleTextAttribute(hConsole, 14);
					cout << "\n(" << i+1 << ") ";
					SetConsoleTextAttribute(hConsole, 15);
					cout << "- " << Inventario[i].Nombre << " (Nivel " << Inventario[i].nivel << ")"; 
				}
			}
		}
	}
	cout << "\n" << texto2 << "\n";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "(1)";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "  Volver  ";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "(2)  ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Usar item";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "  (3)";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "  Soltar item" << "\n"; 
	texto2 = "";
	cin >> Opcion;
	if (Opcion == 1){
		if (enCombate == false){
			jugar();
		}
		else{
			if (turnoActivo == false){
				turnoEnemigo = true;
			}
			else{
				turnoEnemigo = false;
			}
			mostCombate();
		}
	}
	else if(Opcion == 2){
		cout << "\nQue numero de item deseas usar? ";
		cin >> Opcion;
		if (Inventario[Opcion].id != 0){ // Significa que el item es valido
			if (Inventario[Opcion].Datos.Tipo == "?"){
				if (Inventario[Opcion].DatosB.Nombre == "?"){
					texto2 = usarItem(Inventario[Opcion].id, Inventario[Opcion].nivel, Opcion);	
				}
				else{
					if (Inventario[Opcion].DatosB.enUso == true){
						texto2 = "Desequipaste " + Inventario[Opcion].DatosB.Nombre + ". Haz vuelto a tu armadura inicial.";
						Inventario[Opcion].DatosB.enUso = false;
						Jugador.ArmaduraEquipada.Nombre = "Armadura del Novato";
						Jugador.ArmaduraEquipada.RESF = 5 + (2 * Jugador.Nivel);
						Jugador.ArmaduraEquipada.RESFG = 0;
						Jugador.ArmaduraEquipada.RESM = 0;
						Jugador.ArmaduraEquipada.RESV = 0;
						Jugador.ArmaduraEquipada.Nvl = Jugador.Nivel;
					}
					else{
						for (int i=0;i<20;i++){
							Inventario[i].DatosB.enUso = false;
						}
						texto2 = equiparArmadura(Opcion);
					}
				}
			}
			else{
				if (Inventario[Opcion].Datos.enUso == true){
					texto2 = "Desequipaste " + Inventario[Opcion].Datos.Nombre + ". Haz vuelto a tu arma principal.";
					Inventario[Opcion].Datos.enUso = false;
					Jugador.ArmaEquipada.Tipo = "Desarmado";
					Jugador.ArmaEquipada.Nombre = "Desarmado";
					Jugador.ArmaEquipada.DMGF = 5 + (2 * Jugador.Nivel);
					Jugador.ArmaEquipada.DMGFG = 0;
					Jugador.ArmaEquipada.DMGM = 0;
					Jugador.ArmaEquipada.DMGV = 0;
					Jugador.ArmaEquipada.Nvl = Jugador.Nivel;
				}
				else{
					for (int i=0;i<20;i++){
						Inventario[i].Datos.enUso = false;
					}
					texto2 = equiparArma(Opcion);
				}
			}
			inv();
		}
		else{
			texto2 = "\nNo puedes usar un item que no tienes.";
			inv();
		}
	}
	else if (Opcion == 3){
		cout << "\nQue numero de item deseas soltar? ";
		cin >> Opcion;
		if (Inventario[Opcion].id != 0){
			if (Inventario[Opcion].Datos.Tipo == "?"){
				if (Inventario[Opcion].DatosB.Nombre == "?"){
					texto2 = "\nHaz soltado " + Inventario[Opcion].Nombre + ".";
					Inventario[Opcion].id = 0;
					Inventario[Opcion].Nombre = "?";
					Inventario[Opcion].nivel = 1;
				}
				else{
					texto2 = "\nHaz soltado " + Inventario[Opcion].DatosB.Nombre + ".";
					if (Inventario[Opcion].DatosB.enUso == true){
						texto2 = "\nHaz soltado " + Inventario[Opcion].DatosB.Nombre + " y por ende, desequipaste esta armadura.";
						Jugador.ArmaduraEquipada.Nombre = "Armadura del Novato";
						Jugador.ArmaduraEquipada.RESF = 5 + (2 * Jugador.Nivel);
						Jugador.ArmaduraEquipada.RESFG = 0;
						Jugador.ArmaduraEquipada.RESM = 0;
						Jugador.ArmaduraEquipada.RESV = 0;
						Jugador.ArmaduraEquipada.Nvl = Jugador.Nivel;
					}
					Inventario[Opcion].DatosB.enUso = false;
					Inventario[Opcion].id = 0;
					Inventario[Opcion].Nombre = "?";
					Inventario[Opcion].DatosB.Nombre = "?";
					Inventario[Opcion].DatosB.RESFG = 0;
					Inventario[Opcion].DatosB.RESV = 0;
					Inventario[Opcion].DatosB.RESM = 0;
				}
			}
			else{
				texto2 = "\nHaz soltado " + Inventario[Opcion].Datos.Nombre + ".";
				if (Inventario[Opcion].Datos.enUso == true){
					texto2 = "\nHaz soltado " + Inventario[Opcion].Datos.Nombre + " y por ende, desequipaste esta arma.";
					Jugador.ArmaEquipada.Tipo = "Desarmado";
					Jugador.ArmaEquipada.Nombre = "Desarmado";
					Jugador.ArmaEquipada.DMGF = 5 + (2 * Jugador.Nivel);
					Jugador.ArmaEquipada.DMGFG = 0;
					Jugador.ArmaEquipada.DMGM = 0;
					Jugador.ArmaEquipada.DMGV = 0;
					Jugador.ArmaEquipada.Nvl = Jugador.Nivel;
				}
				Inventario[Opcion].Datos.enUso = false;
				Inventario[Opcion].id = 0;
				Inventario[Opcion].Nombre = "?";
				Inventario[Opcion].Datos.Tipo = "?";
				Inventario[Opcion].Datos.DMGFG = 0;
				Inventario[Opcion].Datos.DMGV = 0;
				Inventario[Opcion].Datos.DMGM = 0;
			}
			inv();
		}
		else{
			texto2 = "\nNo puedes soltar un item que no tienes.";
			inv();
		}
	}
	else{
		texto2 = "\nLa opcion no es valida.";
		inv();
	}
}

void stats(){ // Chequeo de stats, cada vez que se sube de nivel, las estadisticas aumentan. Tambien chequea la vida
    // HACER UN LOOP HASTA Q YA NO QUEDE MAS XP
	if (Jugador.CantXP >= Jugador.CantXPMaxima){
	    do{
    		Jugador.Nivel++;
    		texto = texto + "\nSubiste al nivel " + std::to_string(Jugador.Nivel) + "!";
			Jugador.CantXP = Jugador.CantXP - Jugador.CantXPMaxima;
			if (Jugador.Nivel <= 15){
				Jugador.CantXPMaxima = Jugador.CantXPMaxima * 1.4;
			}
			else if (Jugador.Nivel <= 30){
				Jugador.CantXPMaxima = Jugador.CantXPMaxima * 1.2;
			}
			else{
				Jugador.CantXPMaxima = Jugador.CantXPMaxima * 1.05;
			}
			//Jugador.CantXPMaxima = Jugador.CantXPMaxima * 1.5; // Cada vez que se sube de nivel, el proceso se vuelve mas "complicado"
			Jugador.SaludMax = Jugador.SaludMax * 1.1;
			Jugador.Salud = Jugador.SaludMax;
			Jugador.puntosDisp = Jugador.puntosDisp + 4;
			if (Jugador.ArmaEquipada.Tipo == "Desarmado"){
				Jugador.ArmaEquipada.DMGF = Jugador.ArmaEquipada.DMGF + 2;
				Jugador.ArmaEquipada.Nvl = Jugador.Nivel;
			}
			if (Jugador.ArmaduraEquipada.Nombre == "Armadura del Novato"){
				Jugador.ArmaduraEquipada.RESF = Jugador.ArmaduraEquipada.RESF + 2;
				Jugador.ArmaduraEquipada.Nvl = Jugador.Nivel;
			}
		}while(Jugador.CantXP > Jugador.CantXPMaxima);
	}
	if (Jugador.Salud > Jugador.SaludMax){
		Jugador.Salud = Jugador.SaludMax;
	}
}

void darPaso(){
	cofreActivo = false;
	mercaderActivo = false;
    srand(time(0));
    int N = ((rand() % 6));
    texto = "Diste un paso.";
    if (N == 0){
    	jugar();
	}
    else if (N == 1){ 
    	mercaderActivo = true;
    	N = ((rand() % 3)); // El mercader puede ofrecer 3 items distintos.
    	if (N == 0){
    		mercaderId = 1;
    		mercaderPrecio = (80 * Jugador.Nivel) + (15 * Jugador.Nivel); 
    		mercaderNombre = "Pocion de Curacion";
    		texto = "Te encontraste con un mercader.\nTe ofrece una pocion de curacion (Nivel " + std::to_string(Jugador.Nivel) + ") por " + std::to_string(mercaderPrecio) + " de oro.\nInteractua para comprar, da un paso para rechazar.";
		}
		else if (N == 1){
  			mercaderId = 2;
    		mercaderPrecio = (140 * Jugador.Nivel) + (30 * Jugador.Nivel); 
    		mercaderNombre = "Pocion del Mentor";
    		texto = "Te encontraste con un mercader.\nTe ofrece una pocion del mentor (Nivel " + std::to_string(Jugador.Nivel) + ") por " + std::to_string(mercaderPrecio) + " de oro.\nInteractua para comprar, da un paso para rechazar.";
		}
		else if (N == 2){
			mercaderId = 7;
    		mercaderPrecio = (120 * Jugador.Nivel) + (25 * Jugador.Nivel); 
    		mercaderNombre = "Pocion del Turno";
    		texto = "Te encontraste con un mercader.\nTe ofrece una pocion del turno (Nivel " + std::to_string(Jugador.Nivel) + ") por " + std::to_string(mercaderPrecio) + " de oro.\nInteractua para comprar, da un paso para rechazar.";
		}
		jugar();
	}
	else if(N == 2){ // Dar oro.
		srand(time(0));
    	N = ((rand() % 250) + 25);
    	srand(time(0));
    	int N2 = ((rand() % (4 * Jugador.Nivel)));
    	texto = "Diste un paso y encontraste oro en el suelo.\nObtuviste " + std::to_string(N) + " de oro y " + std::to_string(N2) + " de XP.";
    	Jugador.Oro = Jugador.Oro + N;
    	Jugador.CantXP = Jugador.CantXP + N2;
    	jugar();
	}
	else if (N == 3){
		combate();
	}
	else if (N == 4){
		srand(time(0));
    	N = ((rand() % 4));
    	if (N == 0){
    		if (region != "Region de Ancaria"){
    			texto = "Diste un paso y pasaste a la Region de Ancaria, hay muy buenas vistas.";	
			}
    		region = "Region de Ancaria";
		}
		else if (N == 1){
			if (region != "Region de Mascarell"){
    			texto = "Diste un paso y pasaste a la Region de Mascarell. El ambiente se siente un poco raro...";	
			}
    		region = "Region de Mascarell";
		}
		else if (N == 2){
			if (region != "Region de los Pantanos"){
    			texto = "Diste un paso y pasaste a la Region de los Pantanos. Hay un fuerte olor a magia oscura.";	
			}
    		region = "Region de los Pantanos";
		}
		else if (N == 3){
			if (region != "Region del Desierto"){
    			texto = "Diste un paso y pasaste a la Region del Desierto, el sol pega demasiado fuerte.";	
			}
    		region = "Region del Desierto";
		}
		jugar();
	}
	else if (N == 5){
		cofreActivo = true;
		texto = "Te haz encontrado con un cofre. Interactua para abrirlo.";
		jugar();
	}
}

void combate(){
	enemigoEnvenenado = false;
	jugadorEnvenenado = false;
	Enemigo.ArmaduraEquipada.RESFG = 0;
	Enemigo.ArmaduraEquipada.RESM = 0;
	Enemigo.ArmaduraEquipada.RESV = 0;
	Enemigo.ArmaEquipada.DMGFG = 0;
	Enemigo.ArmaEquipada.DMGM = 0;
	Enemigo.ArmaEquipada.DMGV = 0;
	enCombate = true;
	turnoEnemigo = false;
	texto2 = "Entraste en un combate. ";
	if (region == "Region de Ancaria"){
		int N;
		seed = seed + 1;
		srand(seed);
		N = ((rand() % 5));
		if (N == 0){
			Enemigo.Nombre = "Mercenario";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 50 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (20 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Espada del Mercenario";
			Enemigo.ArmaEquipada.DMGF = 9 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura de la Realeza y el Honor";
			Enemigo.ArmaduraEquipada.RESF = 6 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESFG = 4 * Enemigo.Nivel;
		}
		else if (N == 1){
			Enemigo.Nombre = "Goblin";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 30 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (14 * Enemigo.Nivel) * 0.5;
			Enemigo.ArmaEquipada.Nombre = "Daga oxidada";
			Enemigo.ArmaEquipada.DMGF = 6 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.DMGV = 4 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Sin armadura";
			Enemigo.ArmaduraEquipada.RESF = 4 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESM = 6 * Enemigo.Nivel;
		}
		else if (N == 2){
			Enemigo.Nombre = "Chaman Goblin";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 45 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (17 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Baculo";
			Enemigo.ArmaEquipada.DMGF = 8 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.DMGM = 10 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Sin armadura";
			Enemigo.ArmaduraEquipada.RESF = 6 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESM = 8 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESV = 5 * Enemigo.Nivel;
		}
		else if (N == 3){
			Enemigo.Nombre = "Lobo";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 30 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (8 * Enemigo.Nivel) * 0.5;
			Enemigo.ArmaEquipada.Nombre = "Garras";
			Enemigo.ArmaEquipada.DMGF = 6 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Sin armadura";
			Enemigo.ArmaduraEquipada.RESF = 5 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESFG = 4 * Enemigo.Nivel;
		}
		else if (N == 4){
			Enemigo.Nombre = "Burro";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 20 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (5 * Enemigo.Nivel) * 0.5;
			Enemigo.ArmaEquipada.Nombre = "Patadas";
			Enemigo.ArmaEquipada.DMGF = 3 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Sin armadura";
			Enemigo.ArmaduraEquipada.RESF = 3 * Enemigo.Nivel;
		}
	}
	else if (region == "Region de Mascarell"){
		int N;
		seed = seed + 1;
		srand(seed);
		N = ((rand() % 5));
		if (N == 0){
			Enemigo.Nombre = "Ladron";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 40 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (16 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Daga poco afilada";
			Enemigo.ArmaEquipada.DMGF = 9 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura desgastada";
			Enemigo.ArmaduraEquipada.RESF = 4 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESFG = 3 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESV = 4 * Enemigo.Nivel;
		}
		else if (N == 1){
			Enemigo.Nombre = "Asesino";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 50 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (20 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Espada";
			Enemigo.ArmaEquipada.DMGF = 12 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.DMGV = 4 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura desgastada";
			Enemigo.ArmaduraEquipada.RESF = 7 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESFG = 5 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESV = 6 * Enemigo.Nivel;
		}
		else if (N == 2){
			Enemigo.Nombre = "Bandido";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 45 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (18 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Daga muy afilada";
			Enemigo.ArmaEquipada.DMGF = 13 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura desgastada";
			Enemigo.ArmaduraEquipada.RESF = 8 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESFG = 3 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESV = 4 * Enemigo.Nivel;
		}
		else if (N == 3){
			Enemigo.Nombre = "Cazadora de Esclavos";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 60 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (22 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Hacha desgastada";
			Enemigo.ArmaEquipada.DMGF = 12 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.DMGFG = 4 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.DMGV = 2 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura del Esclavista";
			Enemigo.ArmaduraEquipada.RESF = 11 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESFG = 5 * Enemigo.Nivel;
		}
		else if (N == 4){
			Enemigo.Nombre = "Cultista";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 55 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (21 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Espada magica";
			Enemigo.ArmaEquipada.DMGF = 7 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.DMGM = 8 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura magica";
			Enemigo.ArmaduraEquipada.RESF = 3 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESM = 8 * Enemigo.Nivel;
		}
	}
	else if (region == "Region de los Pantanos"){
		int N;
		seed = seed + 1;
		srand(seed);
		N = ((rand() % 2));
		if (N == 0){
			Enemigo.Nombre = "Mago oscuro";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 60 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (24 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Baculo de la magia oscura";
			Enemigo.ArmaEquipada.DMGF = 3 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.DMGM = 7 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura del Mago experto";
			Enemigo.ArmaduraEquipada.RESF = 6 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESM = 7 * Enemigo.Nivel;
		}
		else if (N == 1){
			Enemigo.Nombre = "Lider cultista";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 70 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (30 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Libro del saber";
			Enemigo.ArmaEquipada.DMGF = 0;
			Enemigo.ArmaEquipada.DMGM = 10 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura magica encantada";
			Enemigo.ArmaduraEquipada.RESF = 7 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESM = 13 * Enemigo.Nivel;
		}
	}
	else if (region == "Region del Desierto"){
		int N;
		seed = seed + 1;
		srand(seed);
		N = ((rand() % 5));
		if (N == 0){
			Enemigo.Nombre = "Esqueleto";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 70 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (36 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Espada ligera";
			Enemigo.ArmaEquipada.DMGF = 6 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Sin armadura";
			Enemigo.ArmaduraEquipada.RESF = 4 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESV = 13 * Enemigo.Nivel;
		}
		else if (N == 1){
			Enemigo.Nombre = "Mago esqueleto";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 65 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (30 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Espada desgastada magica";
			Enemigo.ArmaEquipada.DMGF = 4 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.DMGM = 8 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Sin armadura";
			Enemigo.ArmaduraEquipada.RESF = 2 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESV = 10 * Enemigo.Nivel;
		}
		else if (N == 2){
			Enemigo.Nombre = "Orco";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 60 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (25 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Hacha de los Orcos";
			Enemigo.ArmaEquipada.DMGF = 9 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura comun";
			Enemigo.ArmaduraEquipada.RESF = 7 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESFG = 7 * Enemigo.Nivel;
		}
		else if (N == 3){
			Enemigo.Nombre = "Chaman Orco";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 65 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (27 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Hacha magica";
			Enemigo.ArmaEquipada.DMGF = 7 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.DMGM = 8 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura comun";
			Enemigo.ArmaduraEquipada.RESF = 6 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESFG = 8 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESM = 2 * Enemigo.Nivel;
		}
		else if (N == 4){
			Enemigo.Nombre = "Lider Orco";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 70 * Enemigo.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (30 * Enemigo.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Hacha del Lider";
			Enemigo.ArmaEquipada.DMGF = 9 * Enemigo.Nivel;
			Enemigo.ArmaEquipada.Nvl = Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura del Lider";
			Enemigo.ArmaduraEquipada.RESF = 10 * Enemigo.Nivel;
			Enemigo.ArmaduraEquipada.RESFG = 11 * Enemigo.Nivel;
		}
	}
	mostCombate();
}

void mostCombate(){
	system("cls");
	cout << "--        " << Enemigo.Nombre << " (" << Enemigo.Salud << "/" << Enemigo.SaludMax << ") (Nivel " << Enemigo.Nivel << ")" << "        --\n";
	cout << "\n" << texto2;
	enemigoEnvenenado = false;
	if (turnoEnemigo == true){
		cout << "\nTurno del enemigo!\n";
		system("pause");
		int multiplicador5 = Jugador.ArmaduraEquipada.RESF * modificadorResistencia;
		int resi = (Jugador.ArmaduraEquipada.RESF + multiplicador5) * 0.5;
		int atk = Enemigo.ArmaEquipada.DMGF - resi;
		int suma = 0;
		srand(time(0));
    	int N = ((rand() % 3));
    	if (atk < 1){
    		atk = 1;
		}
		texto2 = "Sufriste " + std::to_string(atk) + " de ataque fisico";
    	if (N == 0){
    		atk = atk * 2;
    		if (atk < 1){
    			atk = 1;
			}
    		texto2 = "CRITICO! Sufriste " + std::to_string(atk) + " de ataque fisico";
		}
		suma = suma + atk;
		Jugador.Salud = Jugador.Salud - atk;
		if (Enemigo.ArmaEquipada.DMGFG > 0){
			int multiplicador6 = Jugador.ArmaduraEquipada.RESFG * modificadorResistencia;
			resi = (Jugador.ArmaduraEquipada.RESFG + multiplicador6) * 0.7;
			atk = Enemigo.ArmaEquipada.DMGF - resi;
    		if (atk < 1){
    			atk = 1;
			}
			Jugador.Salud = Jugador.Salud - atk;
			texto2 = texto2 + " + " + std::to_string(atk) + " de ataque de fuego";
			suma = suma + atk;
		}
		if (Enemigo.ArmaEquipada.DMGV > 0){
			int multiplicador7 = Jugador.ArmaduraEquipada.RESV * modificadorResistencia;
			resi = (Jugador.ArmaduraEquipada.RESV + multiplicador7) * 0.8;
			atk = Enemigo.ArmaEquipada.DMGV - resi;
			int atk2 = atk * 0.5;
			if (atk2 < 1){
    			atk2 = 1;
			}	
			Jugador.Salud = Jugador.Salud - atk2;
			srand(time(0));
    		int N = ((rand() % 10));
    		if (N == 5 && atk2 != 1){
    			texto2 = texto2 + " + " + std::to_string(atk2) + " de ataque de veneno (Fuiste envenenado)";
    			jugadorEnvenenado = true;
			}
			else{
				texto2 = texto2 + " + " + std::to_string(atk2) + " de ataque de veneno";
			}
			suma = suma + atk2;
		}
		if (Enemigo.ArmaEquipada.DMGM > 0){
			int multiplicador8 = Jugador.ArmaduraEquipada.RESM * modificadorResistencia;
			resi = (Jugador.ArmaduraEquipada.RESM + multiplicador8) * 0.7;
			atk = Enemigo.ArmaEquipada.DMGM - resi;
			if (atk < 1){
    			atk = 1;
			}
			Jugador.Salud = Jugador.Salud - atk;
			texto2 = texto2 + " + " + std::to_string(atk) + " de ataque magico";
			suma = suma + atk;
		}
		texto2 = texto2 + "\nHaz perdido un total de " + std::to_string(suma) + " de vida!";
		if (Jugador.Salud < 1){
			Jugador.Salud = 0;
			texto2 = texto2 + ".\nFuiste derrotado! Haz perdido todo tu inventario (excepto tu arma principal y armadura) y tu oro.";
			texto = "Fuiste derrotado por " + Enemigo.Nombre + " de nivel " + std::to_string(Enemigo.Nivel) + "! Haz perdido todo tu inventario (excepto tu arma principal y armadura) y tu oro.";
		}
		if (jugadorEnvenenado == true && Jugador.Salud > 0){
			texto2 = texto2 + ".\nComo fuiste envenenado, haz perdido un turno.";
			jugadorEnvenenado = false;
		}
		else{
			turnoEnemigo = false;
		}
		mostCombate();
	}
	if (Enemigo.Salud < 1){
		if (mentorActivo == false){
			Jugador.CantXP = Jugador.CantXP + Enemigo.XPDrop;
			texto = "Derrotaste a " + Enemigo.Nombre + " de nivel " + std::to_string(Enemigo.Nivel) + ".\nRecibiste " + std::to_string(Enemigo.XPDrop) + " XP.";
		}
		else{
			Jugador.CantXP = Jugador.CantXP + (Enemigo.XPDrop * 2);
			texto = "Derrotaste a " + Enemigo.Nombre + " de nivel " + std::to_string(Enemigo.Nivel) + ".\nRecibiste " + std::to_string(Enemigo.XPDrop * 2) + " XP (Doble XP por la Pocion del Mentor).";
			mentorActivo = false;
		}
		if (regeneracionFisica > 0){
			texto = texto + "\nRecuperaste " + std::to_string(regeneracionFisica) + " de vida por la regeneracion fisica.";
			Jugador.Salud = Jugador.Salud + regeneracionFisica;
		}
		cout << "\n\n";
		system("pause");
		obtenerLoot();
	}
	else{
		if (Jugador.Salud < 1){
			Jugador.muertes++;
			cout << "\n\n";
			system("pause");
			for (int i=0;Inventario.size();i++){
				if (Inventario[i].Datos.enUso == false){
					if (Inventario[i].DatosB.enUso == false){
						Inventario[i].id = 0;
						Inventario[i].nivel = 1;
						Inventario[i].Nombre = "?";
						Inventario[i].Datos.Nombre = "?";
						Inventario[i].Datos.Tipo = "?";
						Inventario[i].Datos.DMGFG = 0;
						Inventario[i].Datos.DMGV = 0;
						Inventario[i].Datos.DMGM = 0;
						Inventario[i].DatosB.Nombre = "?";		
						Inventario[i].DatosB.RESFG = 0;	
						Inventario[i].DatosB.RESV = 0;
						Inventario[i].DatosB.RESM = 0;
					}
				}
			}
			Jugador.Oro = Jugador.Oro / 2;
			Jugador.Salud = Jugador.SaludMax;
			jugar();
		}
		else{
			cout << "\n\n--        " << Jugador.Nombre << " (" << Jugador.Salud << "/" << Jugador.SaludMax << ") (Nivel " << Jugador.Nivel << ")" << "        --\n";
			cout << "\n" << "(1)  Atacar  (2)  Usar item  (3)  Ver estadisticas del enemigo y tuyas" << "\n";
		}
	}
	cin >> Opcion;
	if (Opcion == 1){
		int multiplicador1 = Jugador.ArmaEquipada.DMGF * modificadorAtaque;
		int resi = Enemigo.ArmaduraEquipada.RESF * 0.5;
		int atk = (Jugador.ArmaEquipada.DMGF + multiplicador1) - resi;
		int suma = 0;
	    if (atk < 1){
    		atk = 1;
		}
		texto2 = "Hiciste " + std::to_string(atk) + " de ataque fisico";
		srand(time(0));
    	int N = ((rand() % 3));
    	if (N == 0){
    		atk = atk * 2;
    		if (atk < 1){
    			atk = 1;
			}
			texto2 = "CRITICO! Hiciste " + std::to_string(atk) + " de ataque fisico";
		}
		suma = suma + atk;
		Enemigo.Salud = Enemigo.Salud - atk;
		if (Jugador.ArmaEquipada.DMGFG > 0){
			int multiplicador2 = Jugador.ArmaEquipada.DMGFG * modificadorAtaque;
			resi = Enemigo.ArmaduraEquipada.RESFG * 0.7;
			atk = (Jugador.ArmaEquipada.DMGFG + multiplicador2) - resi;
			if (atk < 1){
    			atk = 1;
			}
			Enemigo.Salud = Enemigo.Salud - atk;
			texto2 = texto2 + " + " + std::to_string(atk) + " de ataque de fuego";
			suma = suma + atk;
		}
		if (Jugador.ArmaEquipada.DMGV > 0){
			int multiplicador3 = Jugador.ArmaEquipada.DMGV * modificadorAtaque;
			resi = Enemigo.ArmaduraEquipada.RESV * 0.8;
			atk = (Jugador.ArmaEquipada.DMGV + multiplicador3) - resi;
			int atk2 = atk * 0.5;
			if (atk2 < 1){
    			atk2 = 1;
			}
			Enemigo.Salud = Enemigo.Salud - atk2;
    		int N = ((rand() % 10));
    		if (N == 5 && atk2 != 1){
    			texto2 = texto2 + " + " + std::to_string(atk2) + " de ataque de veneno (Enemigo envenenado!)";
    			enemigoEnvenenado = true;
			}
			else{
				texto2 = texto2 + " + " + std::to_string(atk2) + " de ataque de veneno";
			}
			suma = suma + atk2;
		}
		if (Jugador.ArmaEquipada.DMGM > 0){
			int multiplicador4 = Jugador.ArmaEquipada.DMGM * modificadorAtaque;
			resi = Enemigo.ArmaduraEquipada.RESM * 0.7;
			atk = (Jugador.ArmaEquipada.DMGM + multiplicador4) - resi;
			if (atk < 1){
    			atk = 1;
			}
			Enemigo.Salud = Enemigo.Salud - atk;
			texto2 = texto2 + " + " + std::to_string(atk) + " de ataque magico";
			suma = suma + atk;
		}
		texto2 = texto2 + "\nEl enemigo ha perdido un total de " + std::to_string(suma) + " de vida!";
		if (Enemigo.Salud < 1){
			Enemigo.Salud = 0;
			texto2 = texto2 + ".\nEl enemigo fue derrotado!";
		}
		else{
			if (enemigoEnvenenado == true){
				texto2 = texto2 + ".\nComo envenenaste al enemigo, el enemigo ha perdido un turno.";
			}
			else{
				if (turnoActivo == false){
					turnoEnemigo = true;
					texto2 = texto2 + ".\nAhora es turno del enemigo.";
				}
				else{
					turnoActivo = false;
					texto2 = texto2 + ".\nComo tienes una Pocion del Turno activa, el enemigo ha perdido un turno.";
				}
			}
		}
		mostCombate();
	}
	else if (Opcion == 2){
		inv();
	}
	else if (Opcion == 3){
		int multiplicador1 = Jugador.ArmaEquipada.DMGF * modificadorAtaque;
		int multiplicador2 = Jugador.ArmaEquipada.DMGFG * modificadorAtaque;
		int multiplicador3 = Jugador.ArmaEquipada.DMGV * modificadorAtaque;
		int multiplicador4 = Jugador.ArmaEquipada.DMGM * modificadorAtaque;
		int multiplicador5 = Jugador.ArmaduraEquipada.RESF * modificadorResistencia;
		int multiplicador6 = Jugador.ArmaduraEquipada.RESFG * modificadorResistencia;
		int multiplicador7 = Jugador.ArmaduraEquipada.RESV * modificadorResistencia;
		int multiplicador8 = Jugador.ArmaduraEquipada.RESM * modificadorResistencia;
		cout << "\n--        Estadisticas de " << Enemigo.Nombre << "        --\n";
		cout << "Experiencia dropeada al ganar el combate: " << Enemigo.XPDrop << " XP";
		cout << "\nArma equipada: " << Enemigo.ArmaEquipada.Nombre << " (F. " << Enemigo.ArmaEquipada.DMGF << " - FG. " << Enemigo.ArmaEquipada.DMGFG << " - V. " << Enemigo.ArmaEquipada.DMGV << " - M. " << Enemigo.ArmaEquipada.DMGM << ")"; 
		cout << "\nArmadura equipada: " << Enemigo.ArmaduraEquipada.Nombre << " (F. " << Enemigo.ArmaduraEquipada.RESF << " - FG. " << Enemigo.ArmaduraEquipada.RESFG << " - V. " << Enemigo.ArmaduraEquipada.RESV << " - M. " << Enemigo.ArmaduraEquipada.RESM << ")\n"; 
		cout << "\n--        Estadisticas de " << Jugador.Nombre << "        --\n";
		cout << "Arma equipada: " << Jugador.ArmaEquipada.Nombre << " (F. " << Jugador.ArmaEquipada.DMGF + multiplicador1 << " - FG. " << Jugador.ArmaEquipada.DMGFG + multiplicador2 << " - V. " << Jugador.ArmaEquipada.DMGV + multiplicador3 << " - M. " << Jugador.ArmaEquipada.DMGM + multiplicador4 << ")"; 
		cout << "\nArmadura equipada: " << Jugador.ArmaduraEquipada.Nombre << " (F. " << Jugador.ArmaduraEquipada.RESF + multiplicador5 << " - FG. " << Jugador.ArmaduraEquipada.RESFG + multiplicador6 << " - V. " << Jugador.ArmaduraEquipada.RESV + multiplicador7 << " - M. " << Jugador.ArmaduraEquipada.RESM + multiplicador8 << ")\n\n"; 
		system("pause");
		mostCombate();
	}
	else{
		texto2 = "La opcion no es valida.";
		mostCombate();
	}
}

void mostStats(){
	system("cls");
	SetConsoleTextAttribute(hConsole, 15);
	cout << "--        Caracteristicas de " << Jugador.Nombre << " (";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "Nivel ";  
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.Nivel << ")         --\n";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\nPuntos disponibles: "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.puntosDisp;
	SetConsoleTextAttribute(hConsole, 15);
	cout << "\n1 -"; 
	SetConsoleTextAttribute(hConsole, 12);
	cout << " Fuerza "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << "- " << Jugador.Caracteristicas.Fuerza << " (Bonus " << setprecision(2) << modificadorAtaque << "x de ataque)";
	cout << "\n2 -";
	SetConsoleTextAttribute(hConsole, 9);
	cout << " Resistencia ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "- " << Jugador.Caracteristicas.Resistencia << " (Bonus " << setprecision(2) << modificadorResistencia << "x de resistencia)";
	cout << "\n3 -";
	SetConsoleTextAttribute(hConsole, 10);
	cout << " Regeneracion Fisica ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "- " << Jugador.Caracteristicas.RegFisica << " (+" << regeneracionFisica << " de salud regenerada luego de un combate)";
	cout << "\n4 -";
	SetConsoleTextAttribute(hConsole, 11);
	cout << " Vitalidad ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "- " << Jugador.Caracteristicas.Vitalidad << " (+" << saludMas << " de salud maxima)";
	cout << "\n\n";
	SetConsoleTextAttribute(hConsole, 12);
	cout << "Salud ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.Salud; 
	SetConsoleTextAttribute(hConsole, 12);
	cout << " / ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.SaludMax;
	SetConsoleTextAttribute(hConsole, 3);
	cout << "\nXP "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.CantXP; 
	SetConsoleTextAttribute(hConsole, 3);
	cout << " / ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.CantXPMaxima;
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\n\nArma equipada: "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.ArmaEquipada.Nombre;
	SetConsoleTextAttribute(hConsole, 14);
	cout << " | "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Nivel ";
	cout << Jugador.ArmaEquipada.Nvl;
	// LA SOLUCION
	// no sera la mejor pero bue, es mejor q nada
	int multiplicador1 = Jugador.ArmaEquipada.DMGF * modificadorAtaque;
	int multiplicador2 = Jugador.ArmaEquipada.DMGFG * modificadorAtaque;
	int multiplicador3 = Jugador.ArmaEquipada.DMGV * modificadorAtaque;
	int multiplicador4 = Jugador.ArmaEquipada.DMGM * modificadorAtaque;
	int multiplicador5 = Jugador.ArmaduraEquipada.RESF * modificadorResistencia;
	int multiplicador6 = Jugador.ArmaduraEquipada.RESFG * modificadorResistencia;
	int multiplicador7 = Jugador.ArmaduraEquipada.RESV * modificadorResistencia;
	int multiplicador8 = Jugador.ArmaduraEquipada.RESM * modificadorResistencia;
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\nAtaque: Fisico "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.ArmaEquipada.DMGF + multiplicador1 << " -"; 
	SetConsoleTextAttribute(hConsole, 12);
	cout << " Fuego ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.ArmaEquipada.DMGFG + multiplicador2 << " -";
	SetConsoleTextAttribute(hConsole, 10);
	cout << " Veneno "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.ArmaEquipada.DMGV + multiplicador3 << " -";
	SetConsoleTextAttribute(hConsole, 9);
	cout << " Magico ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.ArmaEquipada.DMGM + multiplicador4;
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\n\nArmadura equipada: ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.ArmaduraEquipada.Nombre;
	SetConsoleTextAttribute(hConsole, 14);
	cout << " |";
	SetConsoleTextAttribute(hConsole, 15);
	cout << " Nivel " << Jugador.ArmaduraEquipada.Nvl;
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\nResistencia: Fisica "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.ArmaduraEquipada.RESF + multiplicador5 << " -"; 
	SetConsoleTextAttribute(hConsole, 12);
	cout << " Fuego ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.ArmaduraEquipada.RESFG + multiplicador6 << " -";
	SetConsoleTextAttribute(hConsole, 10);
	cout << " Veneno "; 
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.ArmaduraEquipada.RESV + multiplicador7 << " -";
	SetConsoleTextAttribute(hConsole, 9);
	cout << " Magica ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.ArmaduraEquipada.RESM + multiplicador8;
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\n\nCantidad de muertes: ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << Jugador.muertes;
	SetConsoleTextAttribute(hConsole, 12);
	if (Jugador.muertes != 1){
		cout << " muertes";	
	}
	else{
		cout << " muerte";
	}
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\n" << texto2 << "\n" << "(1)  ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Volver  ";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "(2)  ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Asignar puntos" << "\n";
	cin >> Opcion;
	if (Opcion == 1){
		jugar();
	}
	else if (Opcion == 2){
		if (Jugador.puntosDisp > 0){
			cout << "\nQue caracteristica quieres mejorar? ";
			cin >> Opcion;
			// HAY QUE RECORTAR DECIMALES!!!
			if (Opcion == 1){
				Jugador.puntosDisp--;
				modificadorAtaque = modificadorAtaque + 0.01;
				texto2 = "Gastaste un punto en Fuerza.";
				Jugador.Caracteristicas.Fuerza++;
				mostStats();
			}
			else if (Opcion == 2){
				Jugador.puntosDisp--;
				modificadorResistencia = modificadorResistencia + 0.01;
				texto2 = "Gastaste un punto en Resistencia.";
				Jugador.Caracteristicas.Resistencia++;
				mostStats();
			}
			else if (Opcion == 3){
				Jugador.puntosDisp--;
				regeneracionFisica = regeneracionFisica + 10;
				Jugador.Caracteristicas.RegFisica++;
				texto2 = "Gastaste un punto en Regeneracion Fisica.";
				mostStats();
			}
			else if (Opcion == 4){
				// faltaaa
				Jugador.puntosDisp--;
				saludMas = saludMas + 20;
				Jugador.Caracteristicas.Vitalidad++;
				Jugador.SaludMax = Jugador.SaludMax + 20;
				texto2 = "Gastaste un punto en Vitalidad.";
				mostStats();
			}
			else{
				texto2 = "La opcion no es valida.";
				mostStats();
			}
		}
		else{
			texto2 = "No tienes puntos para gastar.";
			mostStats();
		}
	}
	else{
		texto2 = "La opcion no es valida.";
		mostStats();
	}
}

int main() {
	system("title Sacred Lite v1.0.4");
	
	int txt;
	bool seleccionado;
	SetConsoleTextAttribute(hConsole, 6);
	cout << "Bienvenido a Sacred Lite!\n\n";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "Clases disponibles:\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	cout << "1. ";
	SetConsoleTextAttribute(hConsole, 12);
	cout << "Gladiador "; 
	SetConsoleTextAttribute(hConsole, 7);
	cout << " -\nGuerrero con la habilidad y entrenamiento necesarios para usar gran parte de las armas. Es un maestro de la hacha y de las armas pesadas, ademas de ser muy fuerte y resistente.\n\n2. ";
	SetConsoleTextAttribute(hConsole, 12);
	cout << "Enano "; 
	SetConsoleTextAttribute(hConsole, 7);
	cout << "-\nTiene poca resistencia y fuerza, pero se cura mucho luego de una batalla. Aun asi, debe estar equipado correctamente.\n";
	do{
		SetConsoleTextAttribute(hConsole, 14);
		cout << "\nDefina su clase poniendo el numero correspondiente: ";
		SetConsoleTextAttribute(hConsole, 7);
		cin >> txt;
		if (txt == 1 || txt == 2){
			Jugador.Clase = txt;
			seleccionado = true;
		}
		else{
			cout << "\nLa clase es invalida, intente otra vez.";
		}
	}while(seleccionado != true);
	system("cls"); 
	seleccionado = false;
	do{
		SetConsoleTextAttribute(hConsole, 7);
		if (Jugador.Clase == 1){
			cout << "Clase seleccionada: ";
			SetConsoleTextAttribute(hConsole, 12);
			cout << "Gladiador\n";
		}
		else{
			cout << "Clase seleccionada: ";
			SetConsoleTextAttribute(hConsole, 12);
			cout << "Enano\n";
		}
		SetConsoleTextAttribute(hConsole, 14);
		cout << "\nEscriba el nombre del personaje: ";
		SetConsoleTextAttribute(hConsole, 7);
		std::getline(std::cin >> std::ws, Jugador.Nombre);
		seleccionado = true;
	}while(seleccionado != true);
	system("cls");
	// Inicio
	if (Jugador.Clase == 1){
		Jugador.SaludMax = 300;
		Jugador.Caracteristicas.Vitalidad = 5;
		saludMas = 100;
		Jugador.Caracteristicas.Resistencia = 20;
		modificadorResistencia = 0.20;
		Jugador.Caracteristicas.Fuerza = 15;
		modificadorAtaque = 0.15;
		Jugador.Caracteristicas.RegFisica = 2;
		regeneracionFisica = 20;
	}
	else{
		Jugador.SaludMax = 160;
		Jugador.Caracteristicas.Fuerza = 5;
		modificadorAtaque = 0.05;
		Jugador.Caracteristicas.RegFisica = 15;
		regeneracionFisica = 150;
		Jugador.Caracteristicas.Vitalidad = 3;
		saludMas = 60;
	}
	Jugador.Salud = Jugador.SaludMax;
	Jugador.ArmaEquipada.Tipo = "Desarmado";
	Jugador.ArmaEquipada.Nombre = "Desarmado";
	Jugador.ArmaEquipada.DMGF = 7;
	Jugador.ArmaEquipada.Nvl = 1;
	Jugador.ArmaduraEquipada.Nombre = "Armadura del Novato";
	Jugador.ArmaduraEquipada.RESF = 7;
	Jugador.ArmaduraEquipada.Nvl = 1;
	region = "Region de Ancaria";
	texto = "Te despiertas en un valle, completamente desarmado.\nEl mundo parece haber cambiado..";
	
	ItemTMP.id = 1;
	ItemTMP.Nombre = "Pocion de Curacion";
	Inventario.push_back(ItemTMP);
	jugar(); 
	return 0;
}
