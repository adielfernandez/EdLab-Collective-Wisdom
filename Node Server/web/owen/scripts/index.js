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

    this.addEvents();
    this.connect();
  }

  CollectiveWisdom.prototype.addEvents = function() {
    this.$contributeBtn.on('click', function() {
      this.$sectionContribute.addClass('is-before');
      this.$sectionThanks.removeClass('is-after');

      this.sendData();
    }.bind(this));

    this.$thanksBtn.on('click', function() {
      this.$sectionThanks.addClass('is-before');
      this.$sectionAbout.removeClass('is-after');
    }.bind(this));
  }

  CollectiveWisdom.prototype.connect = function() {
    // Fake connection
    setTimeout(function() {
      var isSuccess = true;

      this.$header.addClass('is-before');

      if (isSuccess) {
        this.$sectionContribute.removeClass('is-after');
      } else {
        this.$sectionError.removeClass('is-after');
      }
    }.bind(this), 1000);
  };

  CollectiveWisdom.prototype.sendData = function() {
    var contribution = this.$inputContribution.val();
    var name = this.$inputName.val();

    // Send data here.
    console.log(contribution, name);
  }

  return new CollectiveWisdom();
})();

