(function() {
  var CollectiveWisdom = function() {
    this.$header = $('header');
    this.$sectionContribute = $('.section-contribute');
    this.$sectionError = $('.section-error');
    this.$sectionThanks = $('.section-thank-you');
    this.$sectionAbout = $('.section-about');

    this.$contributeBtn = $('[data-ref="contribute-btn"]');
    this.$thanksBtn = $('[data-ref="thanks-btn"]');

    this.$inputContribution = $('[data-ref="input-contribution"]');
    this.$inputName = $('[data-ref="input-name"]');
    this.$inputTag = $('[data-ref="input-tag"]');

    this.addEvents();

    //waits 2000ms before moving past landing page
    this.welcome();




  }

  //Initialize socket and get data
  var socket = io.connect('http://' + window.location.hostname + ':8080');

  // Listen for the initial connection event with the server
  // Normally  you can give a message any name you want, but this is a special case.
  // 'connect' is a reserved message name for SocketIO.
  socket.on('connect', function() {
    console.log('connected!');
  });

  //listen for heartbeats to check connection
  var lastHeartbeatTime = 0;
  var connected = false;
  var hasContributed = false;

  socket.on('heartbeat', function(data) {
    
    var d = new Date();
    lastHeartbeatTime = d.getTime();

    // console.log('Heartbeat received at: ' + lastHeartbeatTime);

  });


  CollectiveWisdom.prototype.addEvents = function() {
    this.$contributeBtn.on('click', function() {
      this.$sectionContribute.addClass('is-before');
      this.$sectionThanks.removeClass('is-after');

      console.log("contribution button clicked");
      hasContributed = true;

      this.sendData();
    }.bind(this));

    this.$thanksBtn.on('click', function() {
      this.$sectionThanks.addClass('is-before');
      this.$sectionAbout.removeClass('is-after');
    }.bind(this));
  }

  CollectiveWisdom.prototype.welcome = function() {
    // Fake connection
    setTimeout(function() {

      this.$header.addClass('is-before');
      this.$sectionContribute.removeClass('is-after');

      setInterval(function() {

        var d = new Date();

        //if its been too long, i.e. 300 ms, just a bit over OF's heartbeat rate (250ms)
        //since we've received a message then we're disconnected
        var timeSinceLastHB = d.getTime() - lastHeartbeatTime;
        
        if(timeSinceLastHB > 1000){
          connected = false;
        } else {
          connected = true;
        }

        console.log("Time since last heartbeat: " + timeSinceLastHB);

        if(connected){

          //hide the error screen
          this.$sectionError.addClass('is-after');

          //bring the contribute screen forward
          this.$sectionContribute.removeClass('is-after');

          //If we've hidden the contribute screen before (because we 
          //weren't connected) bring the it back on screen but
          //only if we haven't contributed yet
          if(!hasContributed){
            this.$sectionContribute.removeClass('is-before');        
          }
          
          // console.log("Connected to server");

        } else {

          //if we haven't received a heartbeat, show the error message
          //but only if we haven't contributed yet
          if(!hasContributed){
            this.$sectionError.removeClass('is-after');
            this.$sectionContribute.addClass('is-before');
          }

          // console.log("NOT Connected to server");

        }
        
      }.bind(this), 300);


    }.bind(this), 2000);
  };





  CollectiveWisdom.prototype.sendData = function() {
    var contribution = this.$inputContribution.val();
    var userName = this.$inputName.val();
    var userTag = this.$inputTag.val();;

    socket.emit('new-message', { name: userName, tag: userTag, message: contribution });

    // Send data here.
    console.log(userName + ", " + userTag + ", " + contribution);
  }

  return new CollectiveWisdom();
})();

