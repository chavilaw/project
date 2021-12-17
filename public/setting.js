var sendButton = function() {
    var button = $('.sendButton');
    button.on('click', function() {
        $(this).hide().html('Sending <span class="loading"></span>').fadeIn('fast');
        setTimeout( function() {
            button.hide().html('Message sent &#10003;').fadeIn('fast');
        }, 3000);
    });
};

sendButton();