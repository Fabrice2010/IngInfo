                          ### Dichiarazione dei set ###
						  

          set T;            # Insieme dei processi
          set P;            # Insieme dei processori

                          ### Dichiarazione dei parametri ###
						  

         param r;                    # Numero di processori
		 
		 param BigM>0;               #costante elevata
		 
         param d{i in T};            # Orario di consegna previsto per ogni processo i
		 
         param p{i in T, j in P};    # Tempo di esecuzione dell'operazione relativa al processo i sul processore j
		 

                          #### Dichiarazione delle variabili di decisione ###
						  

         var t{i in T, j in P} >= 0 ;           # Istante in cui inizia l'esecuzione del processo i sul processore j

         var D{i in T} >= 0 ;                   # Ritardo massimo per il processo i
		 
		 var MaxD >= 0;                         # Variabile che rappresenta il massimo ritardo
		 
		 var y{i in T, k in T, j in P} binary;  # Variabile binaria per la non sovrapposizione
		 

                         ### Definizione dell'obiettivo ###
							
							minimize MaxDelay: MaxD; 
							

                         ### Vincoli ###
								 
				# Ogni processo deve iniziare sull'unico processore:

        subject to StartOnProcessor1{i in T}: t[i,1] = 0;  
		
		
		        # Vincolo di ordine dei processori:

        subject to ProcessorsOrder{i in T, j in 2..r}: t[i,j] >= t[i,j-1] + p[i,j-1];  
		
		
		        # Calcolo del ritardo per ogni processo:

        subject to CalculateDelay{i in T}: D[i] >= t[i,r] + p[i,r] - d[i]; 
		
		
		
		        # Vincolo per il massimo ritardo:
		                        
	    subject to MaxDelayConstraint{i in T}: MaxD >= D[i];   
		
		        # Non sovrapponibilità dei processi su un singolo processore:
                                
        subject to NoOverlap1{i in T, k in T, j in P: i < k}:
                   t[i,j] + p[i,j] <= t[k,j] + y[i,k,j] * BigM ;

        subject to NoOverlap2{i in T, k in T, j in P: i < k}:                 
                t[k,j] + p[k,j] <= t[i,j] + (1 - y[i,k,j]) * BigM ;
				
				
				
