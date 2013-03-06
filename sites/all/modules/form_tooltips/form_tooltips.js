// $Id: form_tooltips.js,v 1.1.2.4 2009/04/07 08:02:49 entrigan Exp $

//How far should tool tip box appear from the mouse?
var offsetX = 20;
var offsetY = 10;

//When document is finished process tooltipping functionality
$(document).ready(function() {

  $('#user-register :input').click(function(){
    
    //$(this.parentNode).find('.description').show();
    $(this.parentNode).find('label.error').hide();

  }
);
  

  //hide all classes that we want tooltipped
  $('form fieldset legend a').click(function(){
    $(this).parent().parent().find('.description').hide();
  });

  $('<div id="desctip"></div>')
  .appendTo('body')
  .hide();

  if($('.description').parent().hasClass('form-item')){
    //add a div at end of field one line
    $('.description').after('<div class="form-tooltip-image"></div>');
    $('.description').hide();
  }
  else if($('.description').prev().hasClass('content-multiple-table')){
    $('.description').hide();
  }
 $('body').click(function(e){
   if(e.target.className == 'form-tooltip-image'){
    
   }else {
     $('#desctip').hide();
   }
 });

  $('.description').next('div.form-tooltip-image').hover(function(e){
      //Create HTML to Tooltipify
      var helptext = form_tooltipsFormtextchild($(this).parent());
      helptext = '<div class = "tool-con">'+helptext+'</div>';
      //alert(helptext);
      //htmltext = '<div class = "des-con">'+helptext+'</div>';
      //Append to page at current mouse position
      //alert(this.className);
      form_tooltipsAppend(helptext,e);
     
    }, function() {
      $('#desctip').hide();
    });

    


  $('.description').prev('.content-multiple-table').hover(function(e){
      //Create HTML to Tooltipify
      var helptext = form_tooltipsFormtextsibling($(this));
      //Append to page at current mouse position
      form_tooltipsAppend(helptext,e);
    }, function() {
      $('#desctip').hide();
    });

  //Make tooltip follow mouse (can impact performance)
  $('.description').parent().mousemove(function(e) {
   // $("#desctip").css('top', e.pageY + offsetY).css('left', e.pageX + offsetX);
    });

});

//Format HTML to go in tip
function form_tooltipsFormtextchild(item) {

  var helptext = item.find('.description').html();
  return helptext;
}

function form_tooltipsFormtextsibling(item) {
  var helptext = item.next('.description').html();
  return helptext;
}

//Append text to body (absolutely positioned)
function form_tooltipsAppend(text,e) {
  //alert(e.pageX +"==="+offsetX);
  if(e.pageX >=1000)
   e.pageX=e.pageX - 300;
  
  $('#desctip').html(text)
  .css('top', e.pageY + offsetY)
  .css('left', e.pageX + offsetX)
  $('#desctip').fadeIn(400);
}
