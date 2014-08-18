	var Pt = 0.28183815;       // transmit power
	var Gt = 1.0;              // transmit antenna gain
	var Gr = 1.0;              // receive antenna
	var freq = 914.0e6;        // frequency
	var sysLoss = 1.0;         // system loss	
	var pathlossExp = 2.0;     // path loss exponent
	var std_db = 2.8;          // shadowing deviation
	var dist0 = 1.0;           // reference distance
	var lambda = 3.0e8/freq;   // lambda 
	var rxThresh_=3.3e-8;      // rx threshold

function freespace(Pt,Gt,Gr,lambda,L,d)
{
var M = lambda / (4 * Math.PI * d);
return (Pt * Gt * Gr * (M * M)) / L;
}
function shadowingPropagation(distance)
{
	
	
	var Pr0 = freespace(Pt,Gt,Gr,lambda,sysLoss,dist0);
	
	var avg_db = -10.0 * pathlossExp * (Math.log(distance/dist0)/Math.log(10));
	var temp=10*(Math.log(rxThresh_/Pr0)/Math.log(10))-avg_db;
	

	return 1 - normalcdf(0,std_db,temp);
		
}
function normalcdf(mean, sigma, to) 
{
    var z = (to-mean)/Math.sqrt(2*sigma*sigma);
    var t = 1/(1+0.3275911*Math.abs(z));
    var a1 =  0.254829592;
    var a2 = -0.284496736;
    var a3 =  1.421413741;
    var a4 = -1.453152027;
    var a5 =  1.061405429;
    var erf = 1-(((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*Math.exp(-z*z);
    var sign = 1;
    if(z < 0)
    {
        sign = -1;
    }
    return (1/2)*(1+sign*erf);
}
function setRxTreshhold( v)
{
	rxThresh = v;
}
function setLambda( v)
{
	lambda = v;
}
function setReferenceDistance( v)
{
	dist0 = v;
}
function setShadowingDeviation( v)
{
	std_db = v;
}
function setPathLossExponent( v)
{
	pathlossExp = v;
}
function setSystemLoss( v)
{
	sysLoss = v;
}
function setFrequency( v)
{
	freq = v;
}
function setTransmitPower( v)
{
	Pt = v;
}
function setTransmitAntennaGain( v)
{
	Gt = v;
}
function setReceiveAntennaGain( v)
{
	Gr = v;
}