$(document).ready(function(){
	//alert('hi');

	var originalPrice    = $('.subtotal span.uc-price').text();
	var origianlPriceInt =  originalPrice.replace("฿", "");
	origianlPriceInt     =  origianlPriceInt.replace(",", "");
	origianlPriceInt      = parseFloat(origianlPriceInt)
	
	 var dollarPrice = $("#usd").text();
	     dollarPrice = parseFloat(dollarPrice);
	 var pondPrice   = $("#pond").text();
	     pondPrice   = parseFloat(pondPrice);
	 var euroPrice   = $("#euro").text();
		 euroPrice   = parseFloat(euroPrice);
	 var yenPrice    = $("#yen").text();
	     yenPrice    = parseFloat(yenPrice);
	 var inrPrice    = $("#inr").text();
	     inrPrice    = parseFloat(inrPrice);

	var calDollarPrice = (origianlPriceInt/dollarPrice).toFixed(3);
	var calPondPrice   = (origianlPriceInt/pondPrice).toFixed(3);
    var calEuroPrice   = (origianlPriceInt/euroPrice).toFixed(3);
	var calYenPrice    = (origianlPriceInt/yenPrice).toFixed(3);
    var calInrPrice    = (origianlPriceInt/inrPrice).toFixed(3);

    
	/********  DATA UPDATION ******/

	$("#usd").text(calDollarPrice);
	$("#pond").text(calPondPrice);
	$("#euro").text(calEuroPrice);
	$("#yen").text(calYenPrice);
	$("#inr").text(calInrPrice);


});