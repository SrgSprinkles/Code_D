(function($) {
	
	/* Go up */
	 jQuery(window).scroll(function () {
	  if(jQuery(this).scrollTop() > 300 ) {
	   jQuery(".go-up").css("bottom","0px");
	  }
	  else {
	   jQuery(".go-up").css("bottom","-60px");
	  }
	  });
	  jQuery(".go-up").click(function(){
	   jQuery("html,body").animate({scrollTop:0},500);
	   return false;
	 });
 

	/* Slime Menu */
	$('.slimmenu').slimmenu({
		resizeWidth: '991',
		collapserTitle: '',
		animSpeed:'medium',
		indentChildren: true,
		childrenIndenter: '<i class="fa fa-chevron-right"></i>'
	});
	// Close Effect Menu
	$(function() {
		$('.collapse-button').click(function() {
			$('.collapse-button').toggleClass('close-menu');
		});
	});
	
	
	/* --------------------------------------------------
	 * document ready
	 * --------------------------------------------------*/
	jQuery(document).ready(function() {
		$("body").show();
		jQuery(window).on("scroll", function() {
			/* fade base scroll position */
			// custom page with background on side
			jQuery('.side-bg').each(function() {
				jQuery(this).find(".image-container").css("height", jQuery(this).find(".image-container").parent().css("height"));
			});
			/* go to anchor */
		});
	});
	
	/* WOW */
	wow = new WOW(
      {
        animateClass: 'animated',
        offset:       100,
        callback:     function(box) {
          console.log("WOW: animating <" + box.tagName.toLowerCase() + ">")
        }
      }
    );
    wow.init();
	
	/* END WOW */
	
	

})(jQuery);

/*
function brandingClick() {
	location.href = 'branding.html';
}

function campaignsClick() {
	location.href = 'integrated_campaigns.html';
}

function socialClick() {
	location.href = 'social_media.html';
}
*/