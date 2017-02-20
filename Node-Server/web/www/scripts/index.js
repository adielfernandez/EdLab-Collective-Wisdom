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


    this.$missingTag = $('[data-ref="tag-error"]');
    this.$missingName = $('[data-ref="name-error"]');
    this.$missingContribution = $('[data-ref="contribution-error"]');

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

      console.log("contribution button clicked");

      //only moves on if function returns true
      //i.e. all 3 fields have been filled out and
      //the data is actually sent
      if( this.sendData() ){

        console.log("Moving on to thank you page");

        hasContributed = true;
        this.$sectionContribute.addClass('is-before');
        this.$sectionThanks.removeClass('is-after');

      } else {

      console.log("Fields not complete");

      }
      


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

        //if its been too long since we've received a message then we're disconnected
        var timeSinceLastHB = d.getTime() - lastHeartbeatTime;
        
        if(timeSinceLastHB > 1000){
          connected = false;
        } else {
          connected = true;
        }

        // console.log("Time since last heartbeat: " + timeSinceLastHB);

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


    }.bind(this), 3000);
  };





  CollectiveWisdom.prototype.sendData = function() {
    var contribution = this.$inputContribution.val();
    var userName = this.$inputName.val();
    var userTagNum = this.$inputTag.val();

    var bHasName = false;
    var bHasTag = false;
    var bHasContribution = false;


    //Check for Contribution Field
    if( contribution.length != 0){
      //hide error text
      bHasContribution = true;
      this.$missingContribution.text("*");
    } else {
      //show error text
      this.$missingContribution.text("* (Required)");
    } 


    //Check for Name Field
    if(userName.length != 0){
      //hide error text
      this.$missingName.text("*");
      bHasName = true;

    } else {
      //show error text
      bHasName = false;
      this.$missingName.text("* (Required)");
      console.log("UserName Field Empty");

    } 


    //Check for Tag Field
    if(userTagNum != -1 ){ //default option has val = -1
      //hide error text
      this.$missingTag.text("*");
      bHasTag = true;
    } else {
      //show error text
      this.$missingTag.text("* (Required)");
      bHasTag = false;
    }
      
    //If all 3 fields are populated, send message and move on
    if(bHasName && bHasTag && bHasContribution){
      //all 3 fields are populated, 

      socket.emit('new-message', { name: userName, tag: userTagNum, message: contribution });

      // Send data here.
      console.log("Sending:  " + userName + ", " + userTagNum + ", " + contribution);

      return true;
     } else {

      //not all fields completed, don't send anything and return false
      return false;
     }

  }




  

  return new CollectiveWisdom();
})();





