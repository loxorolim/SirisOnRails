//Calculo da BER
//2014-07-14


function getHataSRDSuccessRate(distance) {
    return 1 - bit_error_probability(scenario, technology, BIT_RATE, TRANSMITTER_POWER, H_TX, H_RX, distance, SRD);
    
}
function refresh() {
    createTableFromOptions();

    for (var i = 0; i < daps.length; i++) {
        daps[i].refresh();
    }
    if (meshEnabled)
        connectViaMesh();
    
}


//BER para diversas tecnologias
//2014-07-14
/*
//Calculo da taxa de erro de bits
function bit_error_probability(env,technology,bit_rate,transmitter_power,h_tx,h_rx,d,SRD){

    //N_0 para cada tipo de ambiente
   
    var N_0,frequency,E_b, gama_b,bit_error;
 
    var signal_power, signal_power_W;
    if(env == 'Urbano')
        N_0 =  3.1548e-19;
    if(env == 'Suburbano')
        N_0 =   7.0175e-20;    
    if(env == 'Rural')
        N_0 =   4.14275e-21;
    

    //Escolha de tecnologia 802.11g
    if (technology == '802_11_g'){
        frequency = 2400;	//[MHz]

        signal_power = transmitter_power -  loss( frequency , h_tx , h_rx , d, env, SRD);
        
        //signal_power_W = (10.^(signal_power/10))/1000;
        signal_power_W = (Math.pow(10,(signal_power/10)))/1000;
        E_b = signal_power_W / (bit_rate*(Math.pow(10,6)));
               
        gama_b = E_b / N_0;

        //BPSK
        if ((bit_rate == 6) || (bit_rate == 9))
            bit_error = 0.5 * erfc(Math.pow(gama_b,0.5)	);

        //QPSK
        else if ((bit_rate == 12) || (bit_rate == 18))	
            bit_error = 0.5 * erfc(Math.pow(gama_b,0.5)	);

        //16-QAM 
        else if ((bit_rate == 24) || (bit_rate == 36))		
            bit_error = 0.5 * erfc(	Math.pow((0.4*gama_b),0.5)	);
		
        //64-QAM
        else if ((bit_rate == 48) || (bit_rate == 54))	
            bit_error = (1.0/3.0) * erfc(	( Math.pow((9/63)*gama_b ),0.5)  ); 
       
    }

	//Escolha de tecnologia 802.11a	
    if (technology == '802_11_a'){
        frequency = 5400;	//[MHz]
        
        signal_power = transmitter_power - loss( frequency , h_tx , h_rx , d, env, SRD);
        
        //signal_power_W = (10.^(signal_power/10))/1000;
        signal_power_W = (Math.pow(10,(signal_power/10)))/1000;
        E_b = signal_power_W / (bit_rate*(Math.pow(10,6)));
               
        gama_b = E_b / N_0;

        //BPSK
        if ((bit_rate == 6) || (bit_rate == 9))
            bit_error = 0.5 * erfc(Math.pow(gama_b,0.5)	);

        //QPSK
        else if ((bit_rate == 12) || (bit_rate == 18))	
            bit_error = 0.5 * erfc(Math.pow(gama_b,0.5)	);

        //16-QAM 
        else if ((bit_rate == 24) || (bit_rate == 36))		
            bit_error = 0.5 * erfc(	Math.pow((0.4*gama_b),0.5)	);
		
        //64-QAM
        else if ((bit_rate == 48) || (bit_rate == 54))	
            bit_error = (1.0/3.0) * erfc( Math.pow(	( (9/63)*gama_b ),0.5)  );
    
    }
    
    //Escolha de tecnologia 802.15.4 Zigbee
    if (technology == '802_15_4'){
        //frequency = 868;	//[MHz] - Europa
        //frequency = 915;	//[MHz] - EUA
        frequency = 2400;	//[MHz] - Brasileiro 

        signal_power = transmitter_power - loss( frequency , h_tx , h_rx , d, env, SRD);
        
        //signal_power_W = (10.^(signal_power/10))/1000;
        signal_power_W = (Math.pow(10,(signal_power/10)))/1000;
        E_b = signal_power_W / (bit_rate*(Math.pow(10,6)));
               
        gama_b = E_b / N_0;

        //BPSK
        if ((frequency == 868) || (frequency == 915))
            bit_error = 0.5 * erfc(Math.pow(gama_b,0.5)	);

        //OQPSK +/-= QPSK
        else if ((frequency == 2400))	
        bit_error = 0.5 * erfc(Math.pow(gama_b,0.5)	);
    }
    
    return bit_error;
}
function loss( f , h_tx , h_rx , d, environment, SRD){


    var path_loss;
    //Como d <= 20km
    var alpha = 1;

    
    var H_m = Math.min(h_tx,h_rx);
    var H_b = Math.max(h_tx,h_rx);

    
    var a = ( 1.1*log10(f) - 0.7)*Math.min( 10,H_m ) - ( 1.56*log10(f) - 0.8 ) + Math.max( 0,20*log10(H_m/10) );
    var b;
    if (SRD == false)
        b = Math.min( 0.20*log10(H_b/30) );    
    if (SRD == true)
        b = (( (1.1*log10(f)) - 0.7 )*Math.min(10,H_b)) - (( (1.56*log10(f)) - 0.8 )) + Math.max( 0,(20*log10(H_b/10)) );
    if (d <= 0.04)
        path_loss = 32.4 + 20 * log10(f) + 10 * log10((Math.pow(d, 2)) + ((((Math.pow((H_b - H_m), 2))) / (Math.pow(10, 6)))));
        //path_loss = 32.4 + 20*log10(f) + 10*log10((d.^2) + ((((Math.pow((H_b - H_m),2)))/(Math.pow(10,6)))) );
    

    if(d >= 0.1){

        if(environment == 'Urbano'){
            if((30 < f) && (f <= 150))
                path_loss = 69.9 + 26.2*log10(150) - 20*log10(150/f) - 13.82*log10( Math.max(30,H_b) ) + (44.9 - 6.55*log10( Math.max(30,H_b) ))*(Math.pow(log10(d),alpha)) - a - b;           
            if ((150 <  f) && (f <= 1500))
                path_loss = 69.6 + 26.2*log10(f) - 13.82*log10( Math.max(30,H_b) ) + (44.9 - 6.55*log10( Math.max(30,H_b) ))*(Math.pow(log10(d),alpha)) - a - b;           
            if ((1500 < f) && (f <= 2000))
                path_loss = 46.3 + 33.9*log10(f) - 13.82*log10( Math.max(30,H_b) ) + (44.9 - 6.55*log10( Math.max(30,H_b) ))*(Math.pow(log10(d),alpha)) - a - b;            
            if ((2000 < f) && (f <= 3000))
                path_loss = 46.3 + 33.9*log10(2000) + 10*log10(f/2000) - 13.82*log10(Math.max(30,H_b)) + (44.9 - 6.55*log10(Math.max(30,H_b))) * (Math.pow(log10(d),alpha)) - a - b;           
        }

        if(environment == 'Suburbano')
            path_loss = loss( f , h_tx , h_rx , d, 'Urbano', SRD) - 2*(Math.pow((log10( Math.min( Math.max(150,f),2000 ) )/28),2)) - 5.4;
        
        if(environment == 'Rural')
            path_loss = loss( f , h_tx , h_rx , d, 'Urbano', SRD) - (4.78*(Math.pow((log10( Math.min( Math.max(150,f),2000 ) )),2))) + (18.33*log10( Math.min( Math.max(150,f),2000 ) )) - 40.94;
            
    }
    if ((0.04 < d) && (d < 0.1))
        path_loss = (loss( f , h_tx , h_rx , 0.04, environment, SRD)) + ((loss( f , h_tx , h_rx , 0.1, environment, SRD)) - (loss( f , h_tx , h_rx , 0.04, environment, SRD)))*((log10(d)-log10(0.04))/(log10(0.1)-log10(0.04)));
    
    return path_loss;
}
function log10(val) {
    return Math.log(val) / Math.LN10;
}
function erf( x)
{
// constants
    var a1 =  0.254829592;
var a2 = -0.284496736;
var a3 =  1.421413741;
var a4 = -1.453152027;
var a5 =  1.061405429;
var p  =  0.3275911;

// Save the sign of x
var sign = 1;
if (x < 0)
    sign = -1;
x = Math.abs(x);

// A&S formula 7.1.26
var t = 1.0/(1.0 + p*x);
var y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*Math.exp(-x*x);

return sign*y;
}
function erfc(x) {
    return 1 - erf(x);
}
*/