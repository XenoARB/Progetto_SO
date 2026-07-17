# Progetto_SO - Shell con FS

Progetto per il corso di sistemi opertativi del Prof. Grisetti: un file sistem persistente bastato sulla mappatura "mmap". Il FIle System è contenuto in un file immagine e pilotato da una shell.

## Descrizione

Il File System è organizzato in blocchi di dimensione fissa (-Default 4096byte). Un blocco iniziale chiamato SuperBlock con le informazioni di tutto il File System e un indice del primo blocco libero (ancora da allocare), tutti gli altri blocchi hanno i primi 4byte con l'indice del blocco successivo della catena a cui appartiene: file/directory.

Ogni blocco è referenziato tramite indici e mai come indirizzi.

## Requisiti

-gcc con supporto a --std=gnu99
-make
-Sistema POSIX

## Utilizzo

Dopo aver compilato con comando make dentro a src usare il comando ./shell per accedere alla shell e poter lanciare i seguenti comandi


Comandi disponibili:

| Comando | Sintassi | Descrizione |
|---|---|---|
| `format` | `format <file> <size>` | Crea/sovrascrive un'immagine di `<size>` byte (arrotondati per difetto al blocco).**se `<file>` esiste già, il contenuto precedente viene perso.** Viene stampato infine il superblocco con le informazioni del File System|
| `open` | `open <file>` | Riapre un'immagine già formattata in precedenza, verificando la firma (magic number) prima di accettarla. Non tocca il contenuto esistente. Anche qui viene stampato  il superblocco con le informazioni del File System |
| `close` | `close` | Chiude il file system corrente. |
| `mkdir` | `mkdir <nome>` | Crea una sottodirectory nella directory corrente. |
| `touch` | `touch <nome>` | Crea un file vuoto nella directory corrente. |
| `ls` | `ls` | Elenca il contenuto della directory corrente. |
| `cd` | `cd <percorso>` | Cambia directory. Supporta percorsi multi-livello (`a/b/c`), `..`, e percorsi assoluti dalla radice (`/a/b`). |
| `append` | `append <file> <testo>` | Aggiunge testo in coda a un file, allocando nuovi blocchi se necessario. |
| `cat` | `cat <file>` | Stampa il contenuto di un file. |
| `rm` | `rm <nome>` | Rimuove un file, o una directory se è vuota. |



## Stato del Progetto

Tutte le fasi principali del progetto sono concluse rispettando i requisiti minimi imposti dalle specifiche del progetto.

## Autore
Matteo Gargaro - 1856382.