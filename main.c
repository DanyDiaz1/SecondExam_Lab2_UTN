#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define archivoPasajeros "pasajerosAviones.bin"

typedef struct
{
    char dni[9];
    char nombre[30];
    float pesoEquipaje;
    int edad;
    int idAvion;
    int idPasaje;

} registroPasajero;


typedef struct
{
    char nombre[30];
    char dni[9];
    int edad;
} Persona;


typedef struct nodoPasajero
{
    Persona p;
    int idPasaje;
    float pesoEquipaje;
    struct nodoPasajero* sig;

} nodoPasajero;

typedef struct nodoAvion
{
    int idAvion;
    nodoPasajero* pasajeros;
    struct nodoAvion* sig;

} nodoAvion;

nodoAvion* crearNodoAvion(registroPasajero datos);
nodoPasajero* crearNodoPasajero(registroPasajero datos,Persona p);
Persona cargarPersona(registroPasajero datos);
void cargarDatosDelArchivo(nodoAvion** listaAviones);
void insertarAvionAlFinal(nodoAvion** listaAviones,nodoAvion* nuevo);
void insertarPasajeroOrdenadoXDNI(nodoPasajero** listaPasajeros,registroPasajero datos);
int retornarAvionConMasPasajeros(nodoAvion* listaAviones);
void informarPesoTotal(nodoAvion* listaAviones,int id);
void eliminarPasajerosMayores(nodoAvion** listaAviones,int id);
nodoPasajero* borrarPasajeros(nodoPasajero** listaPasajeros);
void pasarPasajerosAArchivo(nodoAvion* listaAviones);


int main()
{
    int opcion=0;
    char seguir;
    nodoAvion* listaAviones=NULL;
    int idMayor;
    int id;


    do
    {
        printf("Ingrese el numero de operacion que desea realizar...\n\n");
        printf("1)Cargar datos y mostrar lista de aviones con sus pasajeros.\n");
        printf("2)Mostrar el avion con la maxima cantidad de pasajeros.\n");
        printf("3)Informar el peso que transporta un avion especifico.\n");
        printf("4)Eliminar pasajeros mayores a 50 anios.\n");
        printf("5)Pasar lista de pasajeros a un archivo x avion.\n");


        scanf("%i",&opcion);

        switch(opcion)
        {
        case 1:

            cargarDatosDelArchivo(&listaAviones);
            mostrarListaDeAviones(listaAviones);

            break;

        case 2:

            if(listaAviones)
            {
                idMayor=retornarAvionConMasPasajeros(listaAviones);
                printf("El avion con mas pasajeros es el avion: %i\n",idMayor);
            }
            else
            {
                printf("La lista no esta cargada, regrese al case 1...\n");
            }

            break;

        case 3:


            if(listaAviones)
            {
                printf("Ingrese el ID que quiere buscar: ");
                scanf("%i",&id);
                informarPesoTotal(listaAviones,id);
            }
            else
            {
                printf("La lista no esta cargada, regrese al case 1...\n");
            }

            break;

        case 4:

            if(listaAviones)
            {
                printf("Ingrese el ID que quiere buscar: ");
                scanf("%i",&id);
                eliminarPasajerosMayores(&listaAviones,id);
                mostrarListaDeAviones(listaAviones);
            }
            else
            {
                printf("La lista no esta cargada, regrese al case 1...\n");
            }

            break;

        case 5:


            if(listaAviones)
            {
                pasarPasajerosAArchivo(listaAviones);
            }
            else
            {
                printf("La lista no esta cargada, regrese al case 1...\n");
            }

            break;

        case 0:

            printf("El programa se cerrara...\n");

            break;

        default:

            printf("Numero de operacion incorrecto, vuelva a intentar\n\n");

            break;
        }

        if(opcion!=0)
        {
            printf("........................................\n");
            printf("Desea realizar otra operacion? [s/n]\n");
            fflush(stdin);
            scanf("%c",&seguir);
            system("pause");
            system("cls");
        }

    }
    while(opcion!=0 && seguir=='s');
    return 0;
}



nodoAvion* crearNodoAvion(registroPasajero datos)
{
    nodoAvion* nuevo=(nodoAvion*)malloc(sizeof(nodoAvion));

    nuevo->idAvion=datos.idAvion;
    nuevo->pasajeros=NULL;
    nuevo->sig=NULL;

    return nuevo;
}

nodoPasajero* crearNodoPasajero(registroPasajero datos,Persona p)
{
    nodoPasajero* nuevo=(nodoPasajero*)malloc(sizeof(nodoPasajero));

    nuevo->idPasaje=datos.idPasaje;
    nuevo->pesoEquipaje=datos.pesoEquipaje;
    nuevo->p=p;
    nuevo->sig=NULL;

    return nuevo;
}

Persona cargarPersona(registroPasajero datos)
{
    Persona p;

    strcpy(p.nombre,datos.nombre);
    p.edad=datos.edad;
    strcpy(p.dni,datos.dni);

    return p;
}


void cargarDatosDelArchivo(nodoAvion** listaAviones)
{
    FILE* archiAviones=fopen(archivoPasajeros,"rb");
    registroPasajero datos;
    nodoAvion* nuevoAvion;
    nodoAvion* aux;

    if(archiAviones!=NULL)
    {
        while(fread(&datos,sizeof(registroPasajero),1,archiAviones)>0)
        {
            if(*listaAviones==NULL)
            {
                nuevoAvion=crearNodoAvion(datos);
                insertarAvionAlFinal(listaAviones,nuevoAvion);
                insertarPasajeroOrdenadoXDNI(&(nuevoAvion->pasajeros),datos);
            }
            else
            {
                aux=*listaAviones;
                while(aux!=NULL && aux->idAvion<datos.idAvion)
                {
                    aux=aux->sig;
                }
                if(aux)
                {
                    insertarPasajeroOrdenadoXDNI(&(aux->pasajeros),datos);
                }
                else
                {
                    nuevoAvion=crearNodoAvion(datos);
                    insertarAvionAlFinal(listaAviones,nuevoAvion);
                    insertarPasajeroOrdenadoXDNI(&(nuevoAvion->pasajeros),datos);
                }
            }
        }
        fclose(archiAviones);
    }
    else
    {
        printf("El archivo no existe o esta dañado\n");
    }
}

void insertarAvionAlFinal(nodoAvion** listaAviones,nodoAvion* nuevo)
{
    nodoAvion* aux;

    if(*listaAviones==NULL)
    {
        *listaAviones=nuevo;
    }
    else
    {
        aux=*listaAviones;

        while(aux->sig)
        {
            aux=aux->sig;
        }
        aux->sig=nuevo;
    }
}


void insertarPasajeroOrdenadoXDNI(nodoPasajero** listaPasajeros,registroPasajero datos)
{
    Persona p=cargarPersona(datos);
    nodoPasajero* nuevo=crearNodoPasajero(datos,p);
    nodoPasajero* ant;
    nodoPasajero* act;

    if(*listaPasajeros==NULL)
    {
        *listaPasajeros=nuevo;
    }
    else
    {
        if(strcmpi(nuevo->p.dni,(*listaPasajeros)->p.dni)>0)
        {
            nuevo->sig=*listaPasajeros;
            *listaPasajeros=nuevo;
        }
        else
        {
            ant=*listaPasajeros;
            act=(*listaPasajeros)->sig;

            while(act && strcmpi(nuevo->p.dni,act->p.dni)>0)
            {
                ant=act;
                act=act->sig;
            }
            ant->sig=nuevo;
            nuevo->sig=act;
        }
    }
}


int retornarAvionConMasPasajeros(nodoAvion* listaAviones)
{
    nodoPasajero* aux;
    int comparador=0;
    int cantPasajeros=0;
    int idConMasPasajeros;

    while(listaAviones)
    {
        aux=listaAviones->pasajeros;

        while(aux)
        {
            cantPasajeros++;
            aux=aux->sig;
        }
        if(cantPasajeros>comparador)
        {
            comparador=cantPasajeros;
            idConMasPasajeros=listaAviones->idAvion;
        }
        listaAviones=listaAviones->sig;
        cantPasajeros=0;
    }

    return idConMasPasajeros;
}



void informarPesoTotal(nodoAvion* listaAviones,int id)
{
    nodoPasajero* aux;
    float sumaPesos=0;

    if(listaAviones)
    {
        while(listaAviones && listaAviones->idAvion<id)
        {
            listaAviones=listaAviones->sig;
        }
        if(listaAviones)
        {
            aux=listaAviones->pasajeros;
            while(aux)
            {
                sumaPesos+=aux->pesoEquipaje;
                aux=aux->sig;
            }
            printf("El peso que se transporta en este avion es: %.2f kg\n",sumaPesos);
        }
        else
        {
            printf("El ID que busca no existe\n");
        }
    }
    else
    {
        printf("La lista de aviones esta vacia\n");
    }
}


void eliminarPasajerosMayores(nodoAvion** listaAviones,int id)
{
    nodoAvion* aux;
    nodoAvion* ant;
    nodoAvion* act;

    if(*listaAviones)
    {
        if((*listaAviones)->idAvion==id)
        {
            aux=*listaAviones;

            if(borrarPasajeros(&(aux->pasajeros))==NULL)
            {
                *listaAviones=(*listaAviones)->sig;
                free(aux);
            }
        }
        else
        {
            ant=*listaAviones;
            act=(*listaAviones)->sig;

            while(act&&act->idAvion<id)
            {
                ant=act;
                act=act->sig;
            }
            if(act)
            {
                if(borrarPasajeros(&(act->pasajeros))==NULL)
                {
                    ant=act->sig;
                    free(act);
                }
            }
        }
    }
}


nodoPasajero* borrarPasajeros(nodoPasajero** listaPasajeros)
{
    nodoPasajero* aux;
    nodoPasajero* ant;
    nodoPasajero* act;

    if(*listaPasajeros)
    {
        if((*listaPasajeros)->p.edad>50)
        {
            aux=*listaPasajeros;
            *listaPasajeros=(*listaPasajeros)->sig;
            free(aux);
        }
        else
        {
            ant=*listaPasajeros;
            act=(*listaPasajeros)->sig;

            while(act)
            {
                while(act&&act->p.edad<=50)
                {
                    ant=act;
                    act=act->sig;
                }
                if(act&&act->p.edad>50)
                {
                    aux=act;
                    ant->sig=act->sig;
                    act=ant->sig;
                    free(aux);
                }
            }
        }
    }
    return *listaPasajeros;
}

void pasarPasajerosAArchivo(nodoAvion* listaAviones)
{
    nodoPasajero* aux;
    Persona p;
    while(listaAviones)
    {
        char nombre[15]="idAvion";
        char numID[4];
        int idAvion=listaAviones->idAvion;
        *numID=idAvion+'0';
        strcat(nombre,numID);
        strcat(nombre,".bin");
        FILE* archiAviones=fopen(nombre,"wb");

        if(archiAviones)
        {
            aux=listaAviones->pasajeros;
            while(aux)
            {
                p=aux->p;
                fwrite(&p,sizeof(Persona),1,archiAviones);
                aux=aux->sig;
            }
            fclose(archiAviones);
        }
        listaAviones=listaAviones->sig;
        idAvion++;
    }
}


void mostrarUnaPersona(Persona p)
{
    printf("Nombre: %s\n",p.nombre);
    printf("Edad: %i\n",p.edad);
    printf("DNI: %s\n",p.dni);
}


void mostrarListaPasajeros(nodoPasajero* listaDePasajeros)
{
    while(listaDePasajeros)
    {
        mostrarUnaPersona(listaDePasajeros->p);
        printf("ID pasaje: %i\n",listaDePasajeros->idPasaje);
        printf("Peso del equipaje: %.2f\n",listaDePasajeros->pesoEquipaje);
        printf("\n");
        listaDePasajeros=listaDePasajeros->sig;
    }
}


void mostrarListaDeAviones(nodoAvion* listaAviones)
{
    while(listaAviones)
    {
        printf("ID avion: %i\n\n",listaAviones->idAvion);
        mostrarListaPasajeros(listaAviones->pasajeros);
        printf("\n");
        listaAviones=listaAviones->sig;
    }
}
