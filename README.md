FORMULAZIONE DEL MODELLO

Per formulare il modello matematico per questo problema , possiamo definire le seguenti variabili:
A
- VARIABILI:
•
𝑡𝑖𝐽: istante in cui inizia l’esecuzione del processo i sul processore j.
•
𝐷ⅈ: ritardo massimo per il processo i.
•
𝑀𝑎𝑥𝐷: massimo ritardo tra tutti i processi.
•
𝑦: variabile binaria per la non sovrapposizione dei processi.
B
- OBIETTIVO:
Minimizzare il massimo ritardo tra tutti i processi :
Minimize MaxDelay: MaxD;
C
- VINCOLI:
1
- Ogni processo deve iniziare l’esecuzione sul processore 1:
𝑡𝑖1=0∀ⅈ∈𝑇
2
- L’esecuzione di un processo su un processore deve iniziare dopo l’esecuzione sul processore precedente:
𝑡𝑖𝐽≥𝑡𝑖(𝐽−1)+𝑃𝑖(𝐽−1) ∀ⅈ∈𝑇 ∀𝐽∈{2,3,…𝑟}
3
- Calcolo del ritardo per ogni processo:
𝐷ⅈ>=𝑡𝑖𝑟+𝑃𝑖𝑟−𝑂𝑟𝑎𝑟ⅈ𝑜𝐷ⅈ𝐶𝑜𝑛𝑠𝑒𝑔𝑛𝑎𝑃𝑟𝑒𝑣ⅈ𝑠𝑡𝑜[ⅈ]
4
- Massimo ritardo per ogni processo:
𝐷ⅈ≥max(𝑂𝑟𝑎𝑟ⅈ𝑜𝐷ⅈ𝐶𝑜𝑛𝑠𝑒𝑔𝑛𝑎𝑃𝑟𝑒𝑣ⅈ𝑠𝑡𝑜[ⅈ]−𝑡𝑖𝑟)
5
- Non sovrapponibilità dei processi :
  𝑡𝑖𝐽+𝑃𝑖𝐽 ≤ 𝑡𝑖𝐽+𝑦𝑖𝑘𝐽∗𝐵ⅈ𝑔𝑀
  t𝑘𝐽+𝑃𝑘𝐽 ≤ 𝑡𝑖𝐽+(1−𝑦𝑖𝑘𝐽)∗𝐵𝑖𝑔𝑀

Per la risoluzione dell’istanza del modello , abbiamo scelto un set task di 3 job con deadline e tempi di esecuzione fissati, schedulati 
su 3 processori. I risultati della schedulazione come mostrato sull’immagine ci fa vedere come all’istante t=0, ogni processo(1,2,3) inizia 
sul processore1. Poi su altri processori inizia con istanti diversi. Inoltre il massimo ritardo è pari a zero cosa che vuol dire che ogni processo è stato
schedulato in modo da rispettare le deadline quindi senza ritardo.

ANALISI DI ALCUNI DATI DELL’ISTANZA
1
- Modifica dei tempi di esecuzione dei processi (P):
Aumentando o diminuendo i tempi di esecuzione, influenzerà direttamente il tempo complessivo richiesto per eseguire ogni processo.
Questo potrebbe portare a modifiche negli istanti di inizio dei processi e nei ritardi massimi.
2
- Modifica orari di consegna previsti:
Cambiare i tempi di consegna previsti potrebbe influenzare il ritardo massimo consentito per ogni processo.
 Una deadline più breve potrebbe richiedere un maggiore sforzo per rispettare i vincoli temporali.
3
- Modifica del numero di processori(r):
Aumentare o diminuire il numero di processori potrebbe cambiare la pianificazione, consentendo una maggiore o minore
parallelizzazione dei processi. Ciò potrebbe influenzare sia gli istanti di inizio dei processi che i ritardi massimi.
4
- Modifica ordine di esecuzione dei processi:
Cambiare l’ordine di esecuzione dei processi potrebbe influenzare l’ordine in cui vengono schedulati su
ciascun processore, potenzialmente alterando il tempo complessivo necessario per completare tutte le operazioni.

CONCLUSIONE
In base ai risultati di risoluzione di un’istanza del nostro modello, possiamo vedere che l’output mostra
come l’algoritmo di scheduling è stato in grado di pianificare tutti i processi senza alcun ritardo rispetto alle deadline previste.

  
