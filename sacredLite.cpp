#include <iostream>
#include <cstdlib>
#include <string.h>
#include <cstring>
#include <cstdio>
#include <time.h> 
#include <iomanip>
#include <windows.h>
using namespace std;

struct Armamento{ // Almacena datos del arma equipada
	string Tipo = "?"; // hacha, espada, arco, daga, desarmado
	string Nombre;
	int Nvl = 1;
	int DMGF = 0; // daÃ±o fisico
	int DMGV = 0; // daÃ±o veneno
	int DMGM = 0; // daÃ±o magico
	int DMGFG = 0; // daÃ±o fuego
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
Item Inventario[10];
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
bool turnoEnemigo = false;
bool jugadorEnvenenado = false;
bool enemigoEnvenenado = false;
bool enCombate = false;

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
	enCombate = false;
	texto2 = "";
	stats();
	system("cls");
	cout << "--        " << region << "        --\n";
	cout << "\n" << texto << "\n\n" << "(1)  Dar un paso  (2)  Inventario  (3)  Interactuar  (4)  Caracteristicas" << "\n"; 
	cout << "\n";
	cout << "|-        " << Jugador.Nombre << "        -|";
	cout << "\n  Nivel " << Jugador.Nivel;
	cout << "\n  XP " << Jugador.CantXP << " / " << Jugador.CantXPMaxima;
	cout << "\n  Salud " << Jugador.Salud << " / " << Jugador.SaludMax; 
	if (mentorActivo == true){
		cout << "\n  Pocion del mentor activa";
	}
	cout << "\n\n  Oro " << Jugador.Oro;
	cout << "\n\n";
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
	if (Jugador.InvUsado != 10){
		for (int i=0;i<10;i++){
			if (Inventario[i].id == 0){
				Inventario[i].id = id;
				Inventario[i].nivel = nivel;
				Inventario[i].Nombre = nombre;
				Inventario[i].DatosB = datos;
				Jugador.InvUsado++;
				break;
			}
		}
	}
	else{
		return "El inventario esta lleno.";
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
	if (Jugador.InvUsado != 10){
		for (int i=0;i<10;i++){
			if (Inventario[i].id == 0){
				Inventario[i].id = id;
				Inventario[i].nivel = nivel;
				Inventario[i].Nombre = nombre;
				Inventario[i].Datos = datos;
				Jugador.InvUsado++;
				break;
			}
		}
	}
	else{
		return "El inventario esta lleno.";
	}
	return "OK.";
}

string agregarItem(int id, int nivel, string nombre){
	if (Jugador.InvUsado != 10){
		for (int i=0;i<10;i++){
			if (Inventario[i].id == 0){
				Inventario[i].id = id;
				Inventario[i].nivel = nivel;
				Inventario[i].Nombre = nombre;
				Jugador.InvUsado++;
				break;
			}
		}
	}
	else{
		return "El inventario esta lleno.";
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
			return "\nUsaste Pocion de Curacion.\nRegeneraste " + std::to_string(cnt) + " de salud."; 
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
	return "Invalido.";
}

void obtenerLoot(){
	if (Jugador.InvUsado < 10){
		Item loot;
		agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
		srand(time(0));
		int N = ((rand() % 26));
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
			loot.Datos.DMGV = 2 * Jugador.Nivel;
			loot.Datos.Nvl = Jugador.Nivel;
			loot.Datos.Tipo = "Daga";
			loot.Datos.Nombre = "Daga comun";
			agregarArma(loot.id, loot.nivel, loot.Nombre, loot.Datos);
		}
		else if (N == 3){
			loot.id = 3;
			loot.nivel = Jugador.Nivel;
			loot.Nombre = "Daga del Novato";
			loot.Datos.DMGF = 9 * Jugador.Nivel;
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
		texto = texto + "\nEl enemigo dropeo " + loot.Nombre + " y ahora esta en tu inventario.";
	}
	else{
		texto = "\nEl enemigo dropeo un item pero tu inventario esta lleno.";
	}
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
		if (Jugador.Oro >= mercaderPrecio && Jugador.InvUsado < 10){
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
		texto = "En este momento no hay nada con lo que interactuar.";
	}
	jugar();
}

void inv(){ // Inventario
	system("cls");
	int cantItems = 0;
	for (int i=0;i<10;i++){
		if (Inventario[i].id != 0){
			cantItems++;
		}
	}
	Jugador.InvUsado = cantItems;
	cout << "--        Inventario de " << Jugador.Nombre << " (" << Jugador.InvUsado << "/10)        --\n";
	for (int i=0;i<10;i++){
		if (Inventario[i].id != 0){
			if (Inventario[i].Datos.Tipo != "?"){
				if (Inventario[i].Datos.enUso == false){
					cout << "\n" << i << " - " << Inventario[i].Nombre << " (Nivel " << Inventario[i].nivel << ") (Ataque | Fisico " << Inventario[i].Datos.DMGF << " - Fuego " << Inventario[i].Datos.DMGFG << " - Veneno " << Inventario[i].Datos.DMGV << " - Magico " << Inventario[i].Datos.DMGM << ")"; 
				}
				else{
					cout << "\n" << i << " - " << Inventario[i].Nombre << " (Nivel " << Inventario[i].nivel << ") (EN USO) (Ataque | Fisico " << Inventario[i].Datos.DMGF << " - Fuego " << Inventario[i].Datos.DMGFG << " - Veneno " << Inventario[i].Datos.DMGV << " - Magico " << Inventario[i].Datos.DMGM << ")"; 
				}
			}
			else{
				if (Inventario[i].DatosB.Nombre != "?"){
					if (Inventario[i].DatosB.enUso == false){
						cout << "\n" << i << " - " << Inventario[i].Nombre << " (Nivel " << Inventario[i].nivel << ") (Resistencia | Fisico " << Inventario[i].DatosB.RESF << " - Fuego " << Inventario[i].DatosB.RESFG << " - Veneno " << Inventario[i].DatosB.RESV << " - Magico " << Inventario[i].DatosB.RESM << ")"; 
					}
					else{
						cout << "\n" << i << " - " << Inventario[i].Nombre << " (Nivel " << Inventario[i].nivel << ") (EN USO) (Resistencia | Fisico " << Inventario[i].DatosB.RESF << " - Fuego " << Inventario[i].DatosB.RESFG << " - Veneno " << Inventario[i].DatosB.RESV << " - Magico " << Inventario[i].DatosB.RESM << ")"; 
					}
				}
				else{
					cout << "\n" << i << " - " << Inventario[i].Nombre << " (Nivel " << Inventario[i].nivel << ")"; 
				}
			}
		}
	}
	cout << "\n" << texto2 << "\n" << "(1)  Volver  (2)  Usar item  (3)  Soltar item" << "\n"; 
	texto2 = "";
	cin >> Opcion;
	if (Opcion == 1){
		if (enCombate == false){
			jugar();
		}
		else{
			turnoEnemigo = true;
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
						for (int i=0;i<10;i++){
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
					for (int i=0;i<10;i++){
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
	    	texto = texto + "\nSubiste al nivel " + std::to_string(Jugador.Nivel) + "!";
    		Jugador.Nivel++;
			Jugador.CantXP = Jugador.CantXP - Jugador.CantXPMaxima;
			if (Jugador.Nivel <= 15){
				Jugador.CantXPMaxima = Jugador.CantXPMaxima * 1.3;
			}
			else if (Jugador.Nivel <= 30){
				Jugador.CantXPMaxima = Jugador.CantXPMaxima * 1.1;
			}
			else{
				Jugador.CantXPMaxima = Jugador.CantXPMaxima * 1.05;
			}
			//Jugador.CantXPMaxima = Jugador.CantXPMaxima * 1.5; // Cada vez que se sube de nivel, el proceso se vuelve mas "complicado"
			Jugador.SaludMax = Jugador.SaludMax * 1.05;
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
	mercaderActivo = false;
    srand(time(0));
    int N = ((rand() % 4));
    texto = "Diste un paso.";
    if (N == 0){
    	jugar();
	}
    else if (N == 1){ 
    	mercaderActivo = true;
    	N = ((rand() % 2)); // El mercader puede ofrecer 3 items distintos.
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
		jugar();
	}
	else if(N == 2){ // Dar oro.
		srand(time(0));
    	N = ((rand() % 250));
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
	texto2 = "Entraste en un combate. ";
	if (region == "Region de Ancaria"){
		int N;
		seed = seed + 1;
		srand(seed);
		N = ((rand() % 4));
		if (N == 0){
			Enemigo.Nombre = "Mercenario";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 35 * Jugador.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (18 * Jugador.Nivel) * 0.5; // Tal vez despues modifique la formula
			Enemigo.ArmaEquipada.Nombre = "Espada del Mercenario";
			Enemigo.ArmaEquipada.DMGF = 6 * Jugador.Nivel;
			Enemigo.ArmaEquipada.Nvl = Jugador.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Armadura de la Realeza y el Honor";
			Enemigo.ArmaduraEquipada.RESF = 3 * Jugador.Nivel;
		}
		else if (N == 1){
			Enemigo.Nombre = "Goblin";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 20 * Jugador.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (10 * Jugador.Nivel) * 0.5;
			Enemigo.ArmaEquipada.Nombre = "Daga oxidada";
			Enemigo.ArmaEquipada.DMGF = 2 * Jugador.Nivel;
			Enemigo.ArmaEquipada.DMGV = 2 * Jugador.Nivel;
			Enemigo.ArmaEquipada.Nvl = Jugador.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Sin armadura";
			Enemigo.ArmaduraEquipada.RESF = 2 * Jugador.Nivel;
			Enemigo.ArmaduraEquipada.RESM = 3 * Jugador.Nivel;
		}
		else if (N == 2){
			Enemigo.Nombre = "Chaman Goblin";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 25 * Jugador.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (12 * Jugador.Nivel) * 0.5; 
			Enemigo.ArmaEquipada.Nombre = "Baculo";
			Enemigo.ArmaEquipada.DMGF = 2 * Jugador.Nivel;
			Enemigo.ArmaEquipada.DMGM = 4 * Jugador.Nivel;
			Enemigo.ArmaEquipada.Nvl = Jugador.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Sin armadura";
			Enemigo.ArmaduraEquipada.RESF = 3 * Jugador.Nivel;
			Enemigo.ArmaduraEquipada.RESM = 5 * Jugador.Nivel;
		}
		else if (N == 3){
			Enemigo.Nombre = "Lobo";
			Enemigo.Nivel = Jugador.Nivel;
			Enemigo.Salud = 20 * Jugador.Nivel;
			Enemigo.SaludMax = Enemigo.Salud;
			Enemigo.XPDrop = (10 * Jugador.Nivel) * 0.5;
			Enemigo.ArmaEquipada.Nombre = "Garras";
			Enemigo.ArmaEquipada.DMGF = 2 * Jugador.Nivel;
			Enemigo.ArmaEquipada.Nvl = Jugador.Nivel;
			Enemigo.ArmaduraEquipada.Nombre = "Sin armadura";
			Enemigo.ArmaduraEquipada.RESF = 3 * Jugador.Nivel;
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
		cout << "\nTurno del enemigo!";
		sleep(2);
		int multiplicador5 = Jugador.ArmaduraEquipada.RESF * modificadorResistencia;
		int resi = (Jugador.ArmaduraEquipada.RESF + multiplicador5) * 0.5;
		int atk = Enemigo.ArmaEquipada.DMGF - resi;
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
		}
		if (Enemigo.ArmaEquipada.DMGV > 0){
			int multiplicador7 = Jugador.ArmaduraEquipada.RESV * modificadorResistencia;
			resi = (Jugador.ArmaduraEquipada.RESV + multiplicador7) * 0.8;
			atk = Enemigo.ArmaEquipada.DMGV - resi;
			int atk2 = atk * 0.5;
			if (atk < 1){
    			atk = 1;
			}	
			Jugador.Salud = Jugador.Salud - atk2;
			srand(time(0));
    		int N = ((rand() % 10));
    		if (N == 5){
    			texto2 = texto2 + " + " + std::to_string(atk2) + " de ataque de veneno (Fuiste envenenado)";
    			jugadorEnvenenado = true;
			}
			else{
				texto2 = texto2 + " + " + std::to_string(atk2) + " de ataque de veneno";
			}
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
		}
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
			cout << "\n\n";
			system("pause");
			for (int i=0;i<10;i++){
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
			Jugador.Oro = 0;
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
		}
		if (Jugador.ArmaEquipada.DMGV > 0){
			int multiplicador3 = Jugador.ArmaEquipada.DMGV * modificadorAtaque;
			resi = Enemigo.ArmaduraEquipada.RESV * 0.8;
			atk = (Jugador.ArmaEquipada.DMGV + multiplicador3) - resi;
			int atk2 = atk * 0.5;
			if (atk < 1){
    			atk = 1;
			}
			Enemigo.Salud = Enemigo.Salud - atk2;
    		int N = ((rand() % 10));
    		if (N == 5){
    			texto2 = texto2 + " + " + std::to_string(atk2) + " de ataque de veneno (Enemigo envenenado!)";
    			enemigoEnvenenado = true;
			}
			else{
				texto2 = texto2 + " + " + std::to_string(atk2) + " de ataque de veneno";
			}
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
		}
		if (Enemigo.Salud < 1){
			Enemigo.Salud = 0;
			texto2 = texto2 + ".\nEl enemigo fue derrotado!";
		}
		else{
			if (enemigoEnvenenado == true){
				texto2 = texto2 + ".\nComo envenenaste al enemigo, el enemigo ha perdido un turno.";
			}
			else{
				turnoEnemigo = true;
				texto2 = texto2 + ".\nAhora es turno del enemigo.";
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
	cout << "--        Caracteristicas de " << Jugador.Nombre << "        --\n";
	cout << "\nPuntos disponibles: " << Jugador.puntosDisp;
	cout << "\n1 - Fuerza - " << Jugador.Caracteristicas.Fuerza << " (Bonus " << setprecision(2) << modificadorAtaque << "x de ataque)";
	cout << "\n2 - Resistencia - " << Jugador.Caracteristicas.Resistencia << " (Bonus " << setprecision(2) << modificadorResistencia << "x de resistencia)";
	cout << "\n3 - Regeneracion Fisica - " << Jugador.Caracteristicas.RegFisica << " (+" << regeneracionFisica << " de salud recuperada luego de una batalla)";
	cout << "\n4 - Vitalidad - " << Jugador.Caracteristicas.Vitalidad << " (+" << saludMas << " de salud maxima)";
	cout << "\n\nArma equipada: " << Jugador.ArmaEquipada.Nombre << " | Nivel " << Jugador.ArmaEquipada.Nvl;
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
	cout << "\nAtaque: Fisico " << Jugador.ArmaEquipada.DMGF + multiplicador1 << " - Fuego " << Jugador.ArmaEquipada.DMGFG + multiplicador2 << " - Veneno " << Jugador.ArmaEquipada.DMGV + multiplicador3 << " - Magico " << Jugador.ArmaEquipada.DMGM + multiplicador4;
	cout << "\n\nArmadura equipada: " << Jugador.ArmaduraEquipada.Nombre << " | Nivel " << Jugador.ArmaduraEquipada.Nvl;
	cout << "\nResistencia: Fisico " << Jugador.ArmaduraEquipada.RESF + multiplicador5 << " - Fuego " << Jugador.ArmaduraEquipada.RESFG + multiplicador6 << " - Veneno " << Jugador.ArmaduraEquipada.RESV + multiplicador7 << " - Magico " << Jugador.ArmaduraEquipada.RESM + multiplicador8;
	cout << "\n" << texto2 << "\n" << "(1)  Volver  (2)  Asignar puntos" << "\n";
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
	int txt;
	bool seleccionado;
	cout << "Bienvenido a Sacred Lite!\n\n";
	cout << "Clases disponibles:\nGladiador (1) - Guerrero con la habilidad y entrenamiento necesarios para usar gran parte de las armas. Es un maestro de la hacha y de las armas pesadas, ademas de ser muy fuerte y resistente.\nEnano (2) - \n";
	do{
		cout << "\nDefina su clase poniendo el numero correspondiente: ";
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
		if (Jugador.Clase == 1){
			cout << "Clase seleccionada: Gladiador\n";
		}
		else{
			cout << "Clase seleccionada: Enano\n";
		}
		cout << "\nDificultades disponibles: Bronce (1) y Plata (2)";
		cout << "\n\nElija su dificultad poniendo el numero correspondiente: ";
		cin >> txt;
		if (txt == 1 || txt == 2){
			Dificultad = txt;
			cout << "\nEscriba el nombre del personaje: ";
			cin >> Jugador.Nombre;
			seleccionado = true;
		}
		else{
			cout << "\nLa dificultad es invalida, intente otra vez.";
		}
	}while(seleccionado != true);
	system("cls");
	// Inicio
	if (Jugador.Clase == 1){
		Jugador.SaludMax = 300;
		Jugador.Caracteristicas.Vitalidad = 5;
		saludMas = 100;
		Jugador.Caracteristicas.Resistencia = 10;
		modificadorResistencia = 0.10;
		Jugador.Caracteristicas.Fuerza = 10;
		modificadorAtaque = 0.10;
	}
	else{
		Jugador.SaludMax = 140;
		Jugador.Caracteristicas.Fuerza = 5;
		modificadorAtaque = 0.05;
		Jugador.Caracteristicas.RegFisica = 15;
		regeneracionFisica = 150;
		Jugador.Caracteristicas.Vitalidad = 2;
		saludMas = 40;
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
	Inventario[0].id = 1;
	Inventario[0].Nombre = "Pocion de Curacion";
	jugar(); 
	return 0;
}
