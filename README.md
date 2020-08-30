# SIR: Simulazione della diffusione di un virus attraverso un automa cellulare
Il progetto SIR da noi realizzato si basa sul concetto di automa cellulare e sui modelli compartimentali introdotti nel secolo scorso per la descrizione della diffusione epidemica. Un automa cellulare, di cui è un celebre esempio il Game of life del matematico inglese John Conway, è un tipo di modello matematico utilizzato per la descrizione di sistemi complessi, in cui viene definita una griglia costituita da celle in grado di evolversi e cambiare il loro stato secondo un preciso algoritmo. Nel nostro caso, tale algoritmo comporta la transizione delle celle, che rappresentano i membri della popolazione interessata, tra suscettibili, infetti, guariti, morti. Tale suddivisione è mutuata, con qualche differenza, da quella del cosiddetto Modello SIR, un sistema di equazioni differenziali in uso correntemente per la descrizione delle epidemie.

## Istruzioni per la compilazione e l’esecuzione del programma

Il programma è suddiviso in numerosi file sorgente, motivo per cui si raccomanda l’uso di CMake. Una volta scaricata, da una finestra terminale sulla cartella SIR è possibile digitare in successione
````````````````
mkdir build 
cd build 
cmake .. 
make
````````````````
Dopo aver atteso il completamento, basta digitare
`````
./SIR
`````
per eseguire il programma. Da terminale viene immediatamente richiesta la dimensione da dare alla propria griglia e viene data la possibilità di inserire parametri di infettività, guarigione e letalità. Se si decide di procedere saltando quest’ultimo step, vengono inseriti valori standard. Si apre dunque una finestra apposita che, dopo aver premuto il tasto S, mostra le istruzioni del programma. Premendo il tasto Q sullo schermo viene visualizzata una griglia. Le celle che la costituiscono sono identificabili attraverso il colore: esse sono blu nel caso di individui non ancora contagiati (suscettibili), rosse nel caso di individui infetti, verdi nel caso dei guariti e infine nere con un piccolo teschio (!!) nel caso dei morti. Inizialmente, tutte le celle sono suscettibili; cliccando sopra a una cella, è possibile infettarla o renderla nuovamente suscettibile. Dopo aver inserito gli infetti, premendo S è possibile far partire la simulazione. Una volta che la pandemia si è fermata, ovvero quando non vi sono più infetti, premendo Q si accede a una finestra in cui è stampato un grafico dell’andamento del contagio, con relativa legenda.

