M = {};

M.url = "m_api";

M.root = $("#m-root");

M.root.append(
  $("<h4>Status:</h4>"),
  M.infoElement = $("<div/>"),
  M.errorsElement = $("<div/>").text("Loading..."),
  $("<h4>Options:</h4>"),
  M.optionsElement = $("<div/>"),
  $("<h4>News:</h4>"),
  M.messagesElement = $("<div/>"),
  $("<h4>Customers:</h4>"),
  M.customersElement = $("<div/>"),
);

M.contestTimeElem = $();
M.customerElements = [];
M.customerFormElements = [];

M.oldState = null;
M.state = null;
M.lastRefreshDate = null;
M.nextRefreshSec = 0;
M.lastMessages = 0;

M.enableAudio = false;
M.enableNotifications = false;

M.sendRequest = function(request, optionalBubbleCoordinates) {
  $.ajax({
    method: "POST",
    url: M.url,
    data: request,
    success: function(state) {
      M.updateState(state, optionalBubbleCoordinates);
    },
    error: function(xhr, status, exc) {
      var content;
      try {
        content = JSON.parse(xhr.responseText);
      } catch (e) {
        content = { error: xhr.statusText && xhr.statusText != "error" ? xhr.statusText : "Network error" };
      }
      M.handleError(content);
    }
  });
}

M.updateState = function(state, optionalBubbleCoordinates) {
  if (state.error) {
    M.handleError(state);
    return;
  }

  M.oldState = M.state;
  M.state = state;
  M.lastRefreshDate = new Date();
  M.nextRefreshSec = state.next_refresh;

  M.showStatus();
  M.showMessages();
  M.showCustomers();
  M.showResultBubble(state.result, optionalBubbleCoordinates);

  M.errorsElement.empty();
};

M.showStatus = function() {
  var clientRealTime = M.lastRefreshDate;
  var serverRealTime = new Date(M.state.real_time * 1000);
  var timeElem = $("<div/>").text(
    "Last update: " + clientRealTime.toLocaleTimeString() + " (client), " +
    serverRealTime.toLocaleTimeString() + " (server), " +
    M.timeToString(M.state.contest_time) + " (contest)");
  if (Math.abs(clientRealTime - serverRealTime) > 7000) {
    timeElem.append($("<b/>").addClass("m-error").text(" (Please check your system time!)"));
  }

  M.infoElement.empty().append(
    timeElem,
    $("<div/>").text("Current contest time: ").append(
      M.contestTimeElem = $("<b/>").text(M.timeToString(M.state.contest_time))),
    $("<div/>").text("Reputation: ").append($("<b/>").text(M.state.reputation))
  );

  if (!M.state.opened) {
    M.infoElement.append($("<br/>"), $("<button/>")
      .text("Open the restaurant").on("click", function() {
        if (confirm("Open the restaurant?")) {
          M.sendRequest({ action: "open" });
        }
      }));
  }
};

M.showMessages = function() {
  M.messagesElement.empty();
  M.state.messages.forEach(function (message) {
    M.messagesElement.append($("<div/>").text(" " + message[1])
      .prepend($("<b/>").text(M.timeToString(message[0]))));
    if (M.lastMessages != 0 && message[0] > M.lastMessages) M.sendNotification("News", { body: message[1] });
  });
  M.state.messages.forEach(function (message) {
    if (message[0] > M.lastMessages) M.lastMessages = message[0];
  });
};

M.showCustomers = function() {
  var oldCustomers = M.oldState && M.oldState.active_customers || [];
  var newCustomers = M.state.active_customers;
  var oldCustomerIds = [];
  var newCustomerIds = [];

  oldCustomers.forEach(function(customer) {
    oldCustomerIds[customer.customer_id] = true;
  });

  newCustomers.forEach(function(customer) {
    newCustomerIds[customer.customer_id] = true;
  });

  oldCustomers.forEach(function(customer) {
    if (!newCustomerIds[customer.customer_id]) {
      M.removeCustomer(customer);
    }
  });

  newCustomers.forEach(function(customer) {
    if (!oldCustomerIds[customer.customer_id]) {
      M.addCustomer(customer);
    }
  });
};

M.removeCustomer = function(customer) {
  var id = customer.customer_id;
  var element = M.customerElements[id];
  if (!element) return;
  M.customerElements[id] = undefined;
  M.customerFormElements[id] = undefined;
  element.slideUp(function() { element.remove(); });

  if (window.console) console.log(customer, M.state.result);

  if (M.currentNotification) {
    try {
      M.currentNotification.close();
      M.currentNotification = undefined;
    } catch (e) {
      console.log(e);
    }
  }
};

M.addCustomer = function(customer) {
  var id = customer.customer_id;

  if (window.console) console.log(customer);

  if (M.customerElements[id]) {
    M.customerElements[id].remove();
  }

  var text = $("<div/>");
  customer.text.split(/(var1|var2)/).forEach(function(part) {
    if (part == "var1" || part == "var2") {
      text.append($("<strong/>").addClass("m-var").text(customer[part]));
    } else {
      text.append(document.createTextNode(part));
    }
  });
  text.append($("<br/>"), $("<br/>"));

  var input = $("<input/>").attr({ size: 40 });
  var expires = customer.expires - 2;
  var form = $("<form/>").attr("id", "m-form-" + id).data("id", id);
  form.append(
    text, input, document.createTextNode(" "),
    $("<span/>").addClass("m-expires").
      data("expires", expires).
      text(M.timeToString(expires - M.state.contest_time)),
    $("<span/>").addClass("m-marker"));
  form.on("submit", function(event) {
    event.preventDefault();
    M.handleSubmit(form);
  });

  M.customerFormElements[id] = form;
  M.customerElements[id] = $("<div/>").appendTo(M.customersElement).hide().append(form, $("<br/>")).slideDown();

  M.currentNotification = M.sendNotification("A customer appears!", {
    body: "Waiting customers: " + M.state.active_customers.length,
    tag: "ipsc2018m"
  });

  if (!document.activeElement || document.activeElement == document.body) {
    input.focus();
  }
};

M.showResultBubble = function(text, optionalBubbleCoordinates) {
  if (!text) return;
  $("<div/>")
    .addClass("m-result")
    .appendTo("body")
    .text(text)
    .offset(optionalBubbleCoordinates || M.messagesElement.offset())
    .animate({ left: "+=20px", top: "-=50px", opacity: 0 }, 2000, "linear",
      function(){ $(this).remove() });
};

M.timeToString = function(time) {
  var s = Math.floor(time) % 60;
  var m = Math.floor(time / 60) % 60;
  var h = Math.floor(time / 60 / 60);
  return (h < 10 ? "0" : "") + h + ":" + (m < 10 ? "0" : "") + m + ":" + (s < 10 ? "0" : "") + s;
};

M.submitAnswer = function(customer_id, answer) {
  var optionalBubbleCoordinates = (M.customerFormElements[customer_id] || $()).find(".m-marker").offset();
  M.sendRequest(
    { action: "submit", customer_id: customer_id, answer: answer },
    optionalBubbleCoordinates);
};

M.handleSubmit = function(form) {
  var customer_id = form.data("id");
  var answer = form.find("input").val();

  if (answer == "") return;

  M.customersElement.find("input").each(function() { this.blur(); });
  M.customersElement.find("input").not(form.find("input")).eq(0).focus();

  M.submitAnswer(customer_id, answer);
};

M.handleError = function(content) {
  if (content.next_refresh) {
    M.nextRefreshSec = content.next_refresh;
  }
  M.sendNotification("Connection error", { body: content.error, tag: "ipsc2018m_error" });
  var div = $("<div/>").addClass("m-error").text(content.error + " ");
  div.prepend($("<b/>").text("Connection error at " + (new Date().toLocaleTimeString()) + ": "))
  if (content.login) {
    div.append($("<a/>").attr("href", content.login).text("(Click here)"));
  }
  M.errorsElement.empty().append(div);
};

M.setNotificationsOption = function(value) {
  M.enableNotifications = value;
  M.optionsElement.find(".m-notifications-input").prop("checked", value);
  localStorage.setItem("2018M_notifications", value ? "1" : "0");
  if (value) $("#m-notifications-error").empty();
  if (value && window.Notification && window.Notification.requestPermission && !M.permissionGranted) {
    var t = setTimeout(function() {
      if (!M.permissionGranted) {
        $("#m-notifications-error").text("Please allow notifications in your browser.");
        M.setNotificationsOption(false);
      }
    }, 1000);
    Notification.requestPermission().then(function(permission) {
      if (permission == 'granted') {
        M.permissionGranted = true;
        if (!M.enableNotifications) M.setNotificationsOption(true);
      }
      if (permission == 'denied') {
        $("#m-notifications-error").text("Browser permission denied.");
        M.setNotificationsOption(false);
      }
      clearTimeout(t);
    });
  }
};

M.setAudioOption = function(value) {
  M.enableAudio = value;
  M.optionsElement.find(".m-audio-input").prop("checked", value);
  localStorage.setItem("2018M_audio", value ? "1" : "0");
  if (value) {
    $("#m-audio-error").empty();
    try {
      var audioElement = M.root.find("audio")[0];
      audioElement.play().then(function() {
        audioElement.pause();
      }, function(e) {
        console.log(e);
        $("#m-audio-error").text("Error playing audio. Please try turning it off and on again.");
      });
    } catch (e) {
      console.log(e);
      $("#m-audio-error").text("Error playing audio.");
    }
  }
};

M.initializeOptions = function() {
  var notifbox = $("<input/>", {type: "checkbox"}).addClass("m-notifications-input")
    .on("change", function() { M.setNotificationsOption(this.checked); });
  var audiobox = $("<input/>", {type: "checkbox"}).addClass("m-audio-input")
    .on("change", function() { M.setAudioOption(this.checked); });

  M.optionsElement.append(
    '<audio loop preload="auto">' +
    '<source src="369148__flying-deer-fx__music-box-the-flea-waltz.mp3" type="audio/mp3">' +
    '<source src="369148__flying-deer-fx__music-box-the-flea-waltz.ogg" type="audio/ogg">' +
    '</audio>');
  if (window.Notification && window.Notification.requestPermission) {
    M.optionsElement.append($("<div/>").append(
      $("<label/>").text(" Enable notifications").prepend(notifbox),
      document.createTextNode(" "),
      $("<span/>").attr("id", "m-notifications-error").addClass("m-error").click(function() { $(this).empty(); })
    ));
  }
  M.optionsElement.append($("<div/>").append(
    $("<label/>").text(" Enable music").prepend(audiobox),
    document.createTextNode(" "),
    $("<span/>").attr("id", "m-audio-error").addClass("m-error").click(function() { $(this).empty(); })
  ));

  if (localStorage.getItem("2018M_notifications") == "1") M.setNotificationsOption(true);
  if (localStorage.getItem("2018M_audio") == "1") M.setAudioOption(true);
};

M.sendNotification = function(title, options) {
  if (M.enableNotifications && M.permissionGranted) {
    try {
      return new Notification(title, options);
    } catch (e) {
      console.log(e);
    }
  }
};

M.regularTick = function() {
  if (!M.state) return;

  var now = new Date();
  var contestTime = M.state.contest_time + Math.floor((now - M.lastRefreshDate) / 1000);
  M.contestTimeElem.text(M.timeToString(contestTime));
  M.root.find(".m-expires").each(function() {
    var self = $(this);
    self.text(M.timeToString(Math.max(0, self.data("expires") - contestTime)));
  });

  if (contestTime > M.nextRefreshSec) {
    M.nextRefreshSec = contestTime + 10;
    M.sendRequest({ action: "refresh" });
  }

  var shouldBePlaying = M.enableAudio && M.state.active_customers.length != 0;
  try {
    var audioElement = M.root.find("audio")[0];
    if (shouldBePlaying) {
      audioElement.play();
    } else {
      audioElement.pause();
      audioElement.currentTime = 0;
    }
  } catch (e) {
    console.log(e);
  }
};

M.initializeOptions();

M.sendRequest({ action: "refresh" });

setInterval(M.regularTick, 500);
