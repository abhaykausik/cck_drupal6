/**
 * Puts the currently highlighted suggestion into the autocomplete field
 */
 

Drupal.behaviors.giri = function (context) {
	
	Drupal.jsAC.prototype.select = function (node) {
		   var customNodeData = node.autocompleteValue;
		   var customNodeDataArr = customNodeData.split("-");
		   //alert('hi');
		   this.input.value = customNodeDataArr['1'];
		$('#edit-auto-hidden').val(customNodeDataArr['0']);
        
      };

	  /**
 * Hides the autocomplete suggestions
 */
Drupal.jsAC.prototype.hidePopup = function (keycode) {
	//alert('bye');
  // Select item if the right key or mousebutton was pressed
  if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {
  // this.input.value = this.selected.autocompleteValue;
		   var customNodeDataArr = this.selected.autocompleteValue.split("-");
		   
		   this.input.value = customNodeDataArr['1'];
		$('#edit-auto-hidden').val(customNodeDataArr['0']);
  }
  // Hide popup
  var popup = this.popup;
  if (popup) {
    this.popup = null;
    $(popup).fadeOut('fast', function() { $(popup).remove(); });
  }
  this.selected = false;
};
	
	
}

