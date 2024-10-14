ho aggiunto:
* il soc preciso tramite un file soc_evaluation.c/.h -  update: tutti gli intervalli sono coerenti
* la callback di errore su i2c per il display
* il blocco nello stato idle se viene premuo il fungo
* gli interrupt con EXTI del fungo_gpio_in e del charge_enable_gpio_in -  update: interrupt funzionano
* la charging_curr come parametro di stop carica nel charging state
* Funzione di timeout nella funzione di gestione del display


ho modificato:
* la TSAC_FAN_HANDLE
* impostato la charging_curr > -0.5 come soglia oltre il quale tornare in idle dallo stato stop_charge (aprire gli AIR)
* tolto la funzine che stava dentro alla callback di un timer che dava il charge enable se ero in stato charge



Tutto da testare.
