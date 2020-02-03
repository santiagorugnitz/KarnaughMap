

#include <iostream>
#include <string>
#include <sstream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"     
#define RED     "\033[31m"     
#define GREEN   "\033[32m"   
#define YELLOW  "\033[33m"    
#define BLUE    "\033[34m"     
#define MAGENTA "\033[35m"     
#define CYAN    "\033[36m"     
#define WHITE   "\033[37m"     
#define BOLDBLACK   "\033[1m\033[30m"    
#define BOLDRED     "\033[1m\033[31m"     
#define BOLDGREEN   "\033[1m\033[32m"    
#define BOLDYELLOW  "\033[1m\033[33m"     
#define BOLDBLUE    "\033[1m\033[34m"     
#define BOLDMAGENTA "\033[1m\033[35m"    
#define BOLDCYAN    "\033[1m\033[36m"      
#define BOLDWHITE   "\033[1m\033[37m"     

using namespace std;


struct listaDoble {
	char dato;
	listaDoble* siguiente;
	listaDoble* anterior;

};


// s no es vacia
listaDoble* pasarALista(string s) {
	int i = 0;
	listaDoble* ret = new listaDoble;

	ret->dato = s[i];
	ret->anterior = NULL;
	ret->siguiente = NULL;
	listaDoble* ultimo = ret;
	i++;
	while (s[i] != '\0') {
		listaDoble* aux = new listaDoble;
		aux->anterior = ultimo;
		aux->siguiente = NULL;
		aux->dato = s[i];
		ultimo->siguiente = aux;
		ultimo = aux;
		i++;
	}
	return ret;
}





//Precondicion: recibe '0' o '1' para cada variable de la formula
listaDoble* interpretar(char a, char b, char c, char d, listaDoble* l) {
	listaDoble* ret = new listaDoble;
	listaDoble* iter = ret;
	ret->dato = l->dato;
	ret->anterior = NULL;
	ret->siguiente = NULL;
	l = l->siguiente;
	while (l != NULL) {
		listaDoble* aux = new listaDoble;
		aux->anterior = iter;
		iter->siguiente = aux;
		aux->siguiente = NULL;
		if (l->dato == 'a')aux->dato = a;
		else if (l->dato == 'b')aux->dato = b;
		else if (l->dato == 'c')aux->dato = c;
		else if (l->dato == 'd')aux->dato = d;
		else aux->dato = l->dato;
		iter = aux;
		l = l->siguiente;
	}
	return ret;
}

listaDoble * buscarLD(listaDoble * l, char x, bool derecha) {
	listaDoble* ret = NULL;
	if (derecha) {
		while (l != NULL && l->dato != x) l = l->siguiente;
		ret = l;
	}
	else {
		while (l != NULL && l->dato != x) l = l->anterior;
		ret = l;
	}
	return ret;
}

// se borra a el (no) y al siguiente y retorna el anterior
// hace el intento y eso es lo que importa
listaDoble* borrarNodoYSiguiente(listaDoble * &l) {
	listaDoble* a = l->anterior;
	listaDoble* s = l->siguiente;
	a->siguiente = s->siguiente;
	if (s->siguiente != NULL) s->siguiente->anterior = a;
	//delete l;
	delete s;
	s = NULL;
	//l = NULL;
	return a;

}

void sacarNot(listaDoble * l) {
	while (l != NULL && l->dato != ')') {
		if (l->dato == '/') {
			if (l->siguiente->dato == '0') l->dato = '1';
			else l->dato = '0';
			listaDoble * borrar = l->siguiente;
			l->siguiente = borrar->siguiente;
			if (borrar->siguiente != NULL)borrar->siguiente->anterior = l;
			delete borrar;
		}
		l = l->siguiente;
	}
}

void sacarMult(listaDoble * l) {
	while (l != NULL && l->dato != ')') {
		if (l->dato == '*') {
			if (l->anterior->dato == '0' || l->siguiente->dato == '0') {
				l->anterior->dato = '0';
			}
			else l->anterior->dato = '1';
			l = borrarNodoYSiguiente(l);
		}
		l = l->siguiente;
	}
}

void sacarSuma(listaDoble * l) {
	while (l != NULL && l->dato != ')') {
		if (l->dato == '+') {
			listaDoble* a = l->anterior;
			listaDoble* s = l->siguiente;
			if (a->dato == '1' || s->dato == '1') {
				a->dato = '1';
			}
			else a->dato = '0';
			l = borrarNodoYSiguiente(l);
		}
		l = l->siguiente;
	}

}



// Precondicion: recibe una cuenta de 0 y 1 Ej (0+1+0+1+1*(/1*1+0))
// Orden: () / * +
int calcular(listaDoble * &l) {
	listaDoble* comienzo = l;
	while (comienzo->siguiente != NULL) {
		listaDoble* ultimo = buscarLD(l, ')', true);
		listaDoble* primero = buscarLD(ultimo, '(', false);
		sacarNot(primero);
		sacarMult(primero);
		sacarSuma(primero);
		primero->dato = primero->siguiente->dato; //guardo el resultado sobre el parentesis
		l = borrarNodoYSiguiente(primero->siguiente);
	}
	if (l->dato == '1')return 1;
	else return 0;
}


int** karnaugh4(listaDoble * formula) {
	int fil = 4;
	int col = 4;
	int** mapa = new int* [fil];
	for (int i = 0; i < fil; i++) mapa[i] = new int[col];


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			char a = '.';
			char b = '.';
			char c = '.';
			char d = '.';
			switch (i) {
			case 0:
				c = '0';
				d = '0';
				break;
			case 1:
				c = '0';
				d = '1';
				break;
			case 2:
				c = '1';
				d = '1';
				break;
			case 3:
				c = '1';
				d = '0';
				break;
			default:
				break;
			}
			switch (j) {
			case 0:
				a = '0';
				b = '0';
				break;
			case 1:
				a = '0';
				b = '1';
				break;
			case 2:
				a = '1';
				b = '1';
				break;
			case 3:
				a = '1';
				b = '0';
				break;
			default:
				break;
			}
			listaDoble* aux = interpretar(a, b, c, d, formula);
			mapa[i][j] = calcular(aux);
		}
	}
	return mapa;
}

struct Rectangulo {
	int h;
	int v;
	int esqi;
	int esqj;
	string formula;
};


void imprimirMatriz(int** m, int var) {
	switch (var)
	{
	case 1:
		break;
	case 2:
		cout << "|" << m[0][0] << "|" << m[0][3] << "|" << endl;
		cout << "|" << m[0][1] << "|" << m[0][2] << "|" << endl;

		break;
	case 3:
		for (int i = 0; i < 4; i += 2) {
			cout << "|";
			for (int j = 0; j < 4; j++)
			{
				cout << m[i][j] << "|";
			}
			cout << endl;
		}
		break;
	default:
		for (int i = 0; i < 4; i++) {
			cout << "|";
			for (int j = 0; j < 4; j++)
			{

				cout << m[i][j] << "|";
			}
			cout << endl;
		}
		break;
	}
}
void imprimirMatriz(int** m, int largo, Rectangulo * r) {
	bool ocupadosEnI[] = { false,false,false,false };
	bool ocupadosEnJ[] = { false,false,false,false };
	for (int i = 0; i < r->v; i++)	ocupadosEnI[(r->esqi + i) % largo] = true;
	for (int i = 0; i < r->h; i++)	ocupadosEnJ[(r->esqj + i) % largo] = true;

	for (int i = 0; i < largo; i++) {
		cout << "|";
		for (int j = 0; j < largo; j++)
		{
			if (ocupadosEnI[i] && ocupadosEnJ[j])
			{
				cout << BOLDRED << m[i][j] << RESET << "|";
			}
			else cout << m[i][j] << "|";
		}
		cout << endl;
	}
}

struct listaR {
	Rectangulo* rect;
	listaR* sig;
};

bool hayVertical(int largo, int posi, int posj, int** tabla, int n, bool esUsados) {
	if (esUsados) {
		int encontrados = 1;
		int i = (posi + 1) % 4;
		while (encontrados <= largo && tabla[i][posj] > 1) {
			i = (i + 1) % 4;
			encontrados++;
		}
		i = (posi + 3) % 4;
		while (encontrados <= largo && tabla[i][posj] > 1) {
			i = (i + 3) % 4;
			encontrados++;
		}
		return encontrados >= largo;
	}
	else {
		int encontrados = 1;
		int i = (posi + 1) % 4;
		while (encontrados <= largo && tabla[i][posj] == n) {
			i = (i + 1) % 4;
			encontrados++;
		}
		i = (posi + 3) % 4;
		while (encontrados <= largo && tabla[i][posj] == n) {
			i = (i + 3) % 4;
			encontrados++;
		}
		return encontrados >= largo;
	}
}

bool hayHorizontal(int largo, int posi, int posj, int** tabla, int n, bool esUsados) {
	if (esUsados) {
		int encontrados = 1;
		int j = (posj + 1) % 4;
		while (encontrados <= largo && tabla[posi][j] > 1) {
			j = (j + 1) % 4;
			encontrados++;
		}
		j = (posj + 3) % 4;
		while (encontrados <= largo && tabla[posi][j] > 1) {
			j = (j + 3) % 4;
			encontrados++;
		}
		return encontrados >= largo;
	}
	else {
		int encontrados = 1;
		int j = (posj + 1) % 4;
		while (encontrados <= largo && tabla[posi][j] == n) {
			j = (j + 1) % 4;
			encontrados++;
		}
		j = (posj + 3) % 4;
		while (encontrados <= largo && tabla[posi][j] == n) {
			j = (j + 3) % 4;
			encontrados++;
		}
		return encontrados >= largo;
	}
}


void hacerFormula(Rectangulo * r, bool esSumaDeProd) { //bien oscuro el metodo
	r->formula = "(";
	bool esPrimero = true;
	bool ab[] = { false,false,false,false };
	bool cd[] = { false,false,false,false };
	ab[r->esqj] = true;
	cd[r->esqi] = true;
	int pos = r->esqj;
	for (int i = 0; i < r->h; i++) {
		ab[pos] = true;
		pos = (pos + 1) % 4;
	}
	pos = r->esqi;
	for (int i = 0; i < r->v; i++) {
		cd[pos] = true;
		pos = (pos + 1) % 4;
	}
	if (esSumaDeProd) {
		if (!((ab[0] || ab[1]) && (ab[2] || ab[3]))) {// se fija si a no cambia
			if (esPrimero)esPrimero = false;
			else r->formula += "*";
			if (ab[0] || ab[1])r->formula += "/a";
			else r->formula += "a";
		}
		if (!((ab[1] || ab[2]) && (ab[0] || ab[3]))) {// se fija si b no cambia
			if (esPrimero)esPrimero = false;
			else r->formula += "*";
			if (ab[0] || ab[3])r->formula += "/b";
			else r->formula += "b";
		}
		if (!((cd[0] || cd[1]) && (cd[2] || cd[3]))) {// se fija si c no cambia
			if (esPrimero)esPrimero = false;
			else r->formula += "*";
			if (cd[0] || cd[1])r->formula += "/c";
			else r->formula += "c";
		}
		if (!((cd[1] || cd[2]) && (cd[0] || cd[3]))) {// se fija si d no cambia
			if (esPrimero)esPrimero = false;
			else r->formula += "*";
			if (cd[0] || cd[3])r->formula += "/d";
			else r->formula += "d";
		}
	}
	else {
		if (!((ab[0] || ab[1]) && (ab[2] || ab[3]))) {// se fija si a no cambia
			if (esPrimero)esPrimero = false;
			else r->formula += "+";
			if (ab[0] || ab[1])r->formula += "a";
			else r->formula += "/a";
		}
		if (!((ab[1] || ab[2]) && (ab[0] || ab[3]))) {// se fija si b no cambia
			if (esPrimero)esPrimero = false;
			else r->formula += "+";
			if (ab[0] || ab[3])r->formula += "b";
			else r->formula += "/b";
		}
		if (!((cd[0] || cd[1]) && (cd[2] || cd[3]))) {// se fija si c no cambia
			if (esPrimero)esPrimero = false;
			else r->formula += "+";
			if (cd[0] || cd[1])r->formula += "c";
			else r->formula += "/c";
		}
		if (!((cd[1] || cd[2]) && (cd[0] || cd[3]))) {// se fija si d no cambia
			if (esPrimero)esPrimero = false;
			else r->formula += "+";
			if (cd[0] || cd[3])r->formula += "d";
			else r->formula += "/d";
		}
	}
	r->formula += ")";

}

Rectangulo* armarRectangulo(int** tabla, int posi, int posj, int** usados, int n, bool agregar) { //(posj+3)%4 mueve la la izquierda y (posj+1)%4 a la derecha
	int x = posi;
	int y = posj;
	if (agregar) {
		for (int i = 2; i >= 0; i--) {
			for (int j = 2; j >= 0; j--) {
				int horizontal = pow(2, j);                                     //i=vertical j=horizontal
				int vertical = pow(2, i);
				bool ok = true;
				int aux;
				posi = x;
				posj = y;
				if (hayVertical(vertical, posi, posj, tabla, n, false)) {
					aux = 1;
					while (tabla[(posi + 3) % 4][posj] == n && aux < vertical) { // mueve la pos hasta llegar al borde superior
						posi = (posi + 3) % 4;
						aux++;
					}
					if (hayHorizontal(horizontal, posi, posj, tabla, n, false)) {
						aux = 1;
						bool hayEspacio = true;
						while (hayEspacio && aux < horizontal) { //mueve la linea vertical hacia la izq para llevar pos a la esquina superior izquierda
							for (int i = 0; i < vertical && hayEspacio; i++) {
								hayEspacio = tabla[(posi + i) % 4][(posj + 3) % 4] == n;
							}
							if (hayEspacio) {
								posj = (posj + 3) % 4; //Ahora esta en esq sup izq
								aux++;
							}
						}
						for (int ii = 0; ii < vertical && ok; ii++) {
							for (int jj = 0; jj < horizontal && ok; jj++) {
								ok = tabla[(posi + ii) % 4][(posj + jj) % 4] == n;
							}
						}
						if (ok) {
							for (int ii = 0; ii < vertical; ii++) {
								for (int jj = 0; jj < horizontal; jj++) {
									usados[(posi + ii) % 4][(posj + jj) % 4] += 1;
								}
							}
							//		cout << "Usados:" << endl;
							//    	  	imprimirMatriz(usados, 4);
							Rectangulo* r = new Rectangulo;
							r->h = horizontal;
							r->v = vertical;
							r->esqi = posi;
							r->esqj = posj;
							hacerFormula(r, n == 1);
							//			cout << r->formula << endl;
							return r;
						}
					}
				}
			}
		}
	}
	else {
		for (int i = 2; i >= 0; i--) {
			for (int j = 2; j >= 0; j--) {
				int horizontal = pow(2, j);                                     //i=vertical j=horizontal
				int vertical = pow(2, i);
				bool ok = true;
				int aux;
				posi = x;
				posj = y;
				if (hayVertical(vertical, posi, posj, usados, n, true)) {
					aux = 1;
					while (usados[(posi + 3) % 4][posj] > 0 && aux < vertical) { // mueve la pos hasta llegar al borde superior
						posi = (posi + 3) % 4;
						aux++;
					}
					if (hayHorizontal(horizontal, posi, posj, usados, n, true)) {
						aux = 1;
						bool hayEspacio = true;
						while (hayEspacio && aux < horizontal) { //mueve la linea vertical hacia la izq para llevar pos a la esquina superior izquierda
							for (int i = 0; i < vertical; i++) {
								hayEspacio = usados[(posi + i) % 4][(posj + 3) % 4] > 1;
							}
							if (hayEspacio) {
								posj = (posj + 3) % 4; //Ahora esta en esq sup izq
								aux++;
							}
						}
						for (int ii = 0; ii < vertical && ok; ii++) {
							for (int jj = 0; jj < horizontal && ok; jj++) {
								ok = usados[(posi + ii) % 4][(posj + jj) % 4] > 1;
							}
						}
						if (ok) {
							for (int ii = 0; ii < vertical; ii++) {
								for (int jj = 0; jj < horizontal; jj++) {
									usados[(posi + ii) % 4][(posj + jj) % 4] += -1;
								}
							}
							//	    		cout << "Usados:" << endl;
							//				imprimirMatriz(usados, 4);
							Rectangulo* r = new Rectangulo;
							r->h = horizontal;
							r->v = vertical;
							r->esqi = posi;
							r->esqj = posj;
							hacerFormula(r, n == 1);
							//				cout << r->formula << endl;
							return r;
						}
					}
				}
			}
		}
	}
}
void agregarRect(Rectangulo * r, listaR * &l) {
	if (l == NULL) {
		l = new listaR;
		l->rect = r;
		l->sig = NULL;
	}
	else {
		listaR* nuevo = new listaR;
		nuevo->rect = r;
		nuevo->sig = l;
		l = nuevo;
	}
}


listaR* borrarRect(Rectangulo * r, listaR * l) {
	listaR* iter = l;
	if (iter->rect->formula._Equal(r->formula)) {
		l = l->sig;
		return l;
	}
	else {
		while (iter->sig != NULL)
		{
			if (iter->sig->rect->formula._Equal(r->formula)) {
				iter->sig = iter->sig->sig;
				return l;
			}
			else {
				iter = iter->sig;
			}
		}
	}

}


string solucion(listaR * l, char operador) {
	if (l == NULL) {
		return "";
	}
	string ret = l->rect->formula;
	l = l->sig;
	while (l != NULL) {
		ret += operador;
		ret += l->rect->formula;
		l = l->sig;
	}
	return ret;
}

void resolverKarnaugh(int ancho, int largo, int n, int** tabla, int** usados) {
	listaR* l = NULL;
	for (int i = 0; i < largo; i++) {
		for (int j = 0; j < ancho; j++) {
			if (tabla[i][j] == n && usados[i][j] == 0) {
				Rectangulo* r = armarRectangulo(tabla, i, j, usados, n, true);
				agregarRect(r, l);
			}
		}
	}
	cout << endl;
	//imprimirMatriz(usados, 4);
	for (int i = 0; i < largo; i++) {
		for (int j = 0; j < ancho; j++) {
			while (usados[i][j] > 1) {
				Rectangulo* r = armarRectangulo(tabla, i, j, usados, n, false);
				borrarRect(r, l);
			}
		}
	}
	listaR* iter = l;
	cout << "Rectangulos:" << endl;
	while (iter != NULL) {
		cout << iter->rect->formula << endl;
		imprimirMatriz(tabla, 4, iter->rect);
		cout << endl;
		iter = iter->sig;
	}
	cout << "Resultado: ";
	if (n == 1)cout << solucion(l, '+') << endl;
	else cout << solucion(l, '*') << endl;
}

/*Errores posibles: 1. Error de parentesis
					2. Error de operadores mezclados
					3. Error en la sintaxis de parentesis
*/

int parentesis(listaDoble * l) {
	int error = 0;
	int parentesis = 0;
	while (l != NULL && parentesis >= 0)
	{
		if (l->dato == '(')
		{
			parentesis++;
		}
		if (l->dato == ')')
		{
			parentesis--;
		}
		l = l->siguiente;
	}
	if (parentesis == 0)
	{
		error = 0;
	}
	else {
		error = 1;
	}
	return error;
}

int repite(listaDoble * l) {
	int error = 0;
	while (l != NULL && error == 0)
	{
		if (l->dato == '(' && l->siguiente != NULL && l->siguiente->dato == ')')
		{
			error = 3;
		}
		if (l->dato == '+' && l->siguiente != NULL && l->siguiente->dato == ')')
		{
			error = 3;
		}
		if (l->dato == '+' && l->anterior != NULL && l->anterior->dato == '(')
		{
			error = 3;
		}
		if (l->dato == '*' && l->siguiente != NULL && l->siguiente->dato == ')')
		{
			error = 3;
		}
		if (l->dato == '*' && l->anterior != NULL && l->anterior->dato == '(')
		{
			error = 3;
		}
		if (l->dato == '/' && l->siguiente != NULL && l->siguiente->dato == ')')
		{
			error = 3;
		}
		if (l->dato == '+' && l->siguiente != NULL && l->siguiente->dato == '*')
		{
			error = 2;
		}
		if (l->dato == '*' && l->siguiente != NULL && l->siguiente->dato == '+')
		{
			error = 2;
		}
		if (l->dato == '/' && l->siguiente != NULL && l->siguiente->dato == '+')
		{
			error = 2;
		}
		if (l->dato == '/' && l->siguiente != NULL && l->siguiente->dato == '*')
		{
			error = 2;
		}
		l = l->siguiente;
	}
	return error;
}

int bienFormado(listaDoble * l) {
	int parentesis1 = parentesis(l);
	int repite1 = repite(l);
	if (parentesis1 == 0)
	{
		if (repite1 == 0)
		{
			return 0;
		}
		else {
			return repite1;
		}
	}
	else {
		return parentesis1;
	}
}

void sacarRepetidos(listaDoble * &l) {
	listaDoble* iter = l;
	iter->anterior = NULL;
	while (iter != NULL)
	{
		//Elimina doble neg
		if (iter->siguiente != NULL && iter->dato == '/' && iter->siguiente->dato == '/')
		{
			listaDoble* borrar1 = iter;
			listaDoble* borrar2 = iter->siguiente;
			iter->anterior->siguiente = iter->siguiente->siguiente;
			delete borrar1;
			delete borrar2;
		}

		if (iter->siguiente != NULL && iter->dato == '+' && iter->siguiente->dato == '+')
		{
			listaDoble* borrar1 = iter->siguiente;
			iter->siguiente = iter->siguiente->siguiente;
			delete borrar1;

		}

		if (iter->siguiente != NULL && iter->dato == ' ' && iter->siguiente->dato == ' ')
		{
			listaDoble* borrar1 = iter->siguiente;
			iter->siguiente = iter->siguiente->siguiente;
			delete borrar1;

		}

		if (iter->siguiente != NULL)
		{
			//el if mas oscuro del mundo
			if ((iter->dato == ')' && iter->siguiente->dato == '(')||(iter->dato == 'a' && iter->siguiente->dato == 'a') || (iter->dato == 'b' && iter->siguiente->dato == 'b') || (iter->dato == 'c' && iter->siguiente->dato == 'c') || (iter->dato == 'd' && iter->siguiente->dato == 'd') || (iter->dato == 'a' && iter->siguiente->dato == 'b') || (iter->dato == 'a' && iter->siguiente->dato == 'c') || (iter->dato == 'a' && iter->siguiente->dato == 'd') || (iter->dato == 'b' && iter->siguiente->dato == 'a') || (iter->dato == 'b' && iter->siguiente->dato == 'c') || (iter->dato == 'b' && iter->siguiente->dato == 'd') || (iter->dato == 'c' && iter->siguiente->dato == 'a') || (iter->dato == 'c' && iter->siguiente->dato == 'b') || (iter->dato == 'c' && iter->siguiente->dato == 'd') || (iter->dato == 'd' && iter->siguiente->dato == 'a') || (iter->dato == 'd' && iter->siguiente->dato == 'b') || (iter->dato == 'd' && iter->siguiente->dato == 'c'))
			{
				listaDoble* agregar = new listaDoble;
				listaDoble* segundo = iter->siguiente;

				agregar->dato = '*';

				segundo->anterior = agregar;
				agregar->siguiente = segundo;

				iter->siguiente = agregar;
				agregar->anterior = iter;
			}
		}
		iter = iter->siguiente;
	}
}

void imprimirFormula(listaDoble * l) {
	while (l != NULL)
	{
		cout << l->dato;
		l = l->siguiente;
	}
	cout << endl;
}

int main()
{
	string f3 = "/a*/b*c*d+/a*b*/c*d+/a*b*c*d+a*b*/c*/d+a*b*/c*d+a*b*c*/d+a*b*c*d";
	string f1 = "(a+b*c)+d*/d";
	string f2 = "(/b+/c)(/a+/b+c)(/a+b+d)";
	bool ok = true;
	string f;
	int seguir = 0;
	while (seguir == 0) {
		cout << "Ingrese el numero de la opcion que desea" << endl;
		cout << "1- Ingresar una formula por consola" << endl;
		cout << "2- Usar (a+b*c)+d*/d" << endl;
		cout << "3- Usar (/b+/c)(/a+/b+c)(/a+b+d)" << endl;
		cout << "4- Usar /a*/b*c*d+/a*b*/c*d+/a*b*c*d+a*b*/c*/d+a*b*/c*d+a*b*c*/d+a*b*c*d" << endl;
		while (ok) {
			int ingreso;
			getline(cin, f);
			stringstream(f) >> ingreso;
			switch (ingreso) {
			case 1:
				getline(cin, f);
				ok = false;
				break;
			case 2:
				f = f1;
				ok = false;
				break;
			case 3:
				f = f2;
				ok = false;
				break;
			case 4:
				f = f3;
				ok = false;
				break;
			default:
				cout << "Ingrese una opcion valida" << endl;
				break;
			}
		}
		ok = true;
		listaDoble* formula = pasarALista("(" + f + ")");
		seguir = bienFormado(formula);
		if (seguir == 0) {
			sacarRepetidos(formula);
			imprimirFormula(formula);
			int** m = karnaugh4(formula);
			cout << "Mapa de Karnaugh:" << endl;
			imprimirMatriz(m, 4);
			int** aux = new int* [4];
			for (int i = 0; i < 4; i++) aux[i] = new int[4];
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					aux[i][j] = 0;
				}
			}
			resolverKarnaugh(4, 4, 1, m, aux);
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					aux[i][j] = 0;
				}
			}
			resolverKarnaugh(4, 4, 0, m, aux);
			cout << endl;
		}
		else {
			cout << "Formula mal formada" << endl;
			if (seguir == 1)
			{
				cout << "Error de parentizacion" << endl;
			}
			else if (seguir == 2) {
				cout << "Operadores mezclados" << endl;
			}
			else if (seguir == 3) {
				cout << "Error en la sintaxis de parentesis" << endl;
			}
			seguir = 0;
		}
	}
	return 0;
}
