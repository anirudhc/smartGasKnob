/**
 * App ID for the skill
 */
var APP_ID = "amzn1.echo-sdk-ams.app.0107949f-f1bd-47e1-989c-1d1a3679df89"; //replace with "amzn1.echo-sdk-ams.app.[your-unique-value-here]";

/**
 * Use Particle API JS to access the temperature / humidity data
 * @see https://docs.particle.io/reference/javascript/
 */
var Particle = require('particle-api-js');
var particle = new Particle();

/**
 * Particle authentication credentials
 * @see https://docs.particle.io/reference/api/#authentication
 */
var PARTICLE_DEVICE_ID = "3a001c001951353337343731";
var PARTICLE_ACCESS_TOKEN = "f201d6859a83aed6b972679706055660827974a7";

/**
 * The AlexaSkill prototype and helper functions
 */
var AlexaSkill = require('./AlexaSkill');

/**
 * ParticleSkill is a child of AlexaSkill.
 * To read more about inheritance in JavaScript, see the link below.
 *
 * @see https://developer.mozilla.org/en-US/docs/Web/JavaScript/Introduction_to_Object-Oriented_JavaScript#Inheritance
 */
var ParticleSkill = function () {
    AlexaSkill.call(this, APP_ID);
};

// Extend AlexaSkill
ParticleSkill.prototype = Object.create(AlexaSkill.prototype);
ParticleSkill.prototype.constructor = ParticleSkill;

ParticleSkill.prototype.eventHandlers.onSessionStarted = function (sessionStartedRequest, session) {
    console.log("ParticleSkill onSessionStarted requestId: " + sessionStartedRequest.requestId
        + ", sessionId: " + session.sessionId);
    // any initialization logic goes here
};

ParticleSkill.prototype.eventHandlers.onLaunch = function (launchRequest, session, response) {
    console.log("ParticleSkill onLaunch requestId: " + launchRequest.requestId + ", sessionId: " + session.sessionId);
    var speechOutput = "Hey Anirudh! I can help you with boiling eggs, boiling milk. I can make you some french fries if you'd like. ";
    var repromptText = "I'm here to help!";
    response.ask(speechOutput, repromptText);
};

ParticleSkill.prototype.eventHandlers.onSessionEnded = function (sessionEndedRequest, session) {
    console.log("ParticleSkill onSessionEnded requestId: " + sessionEndedRequest.requestId
        + ", sessionId: " + session.sessionId);
    // any cleanup logic goes here
};

ParticleSkill.prototype.intentHandlers = {
    // register custom intent handlers
    "ParticleStateIntent": function (intent, session, response) {
        var slot = intent.slots.state;
        var slotValue = slot ? slot.value : "";

        if(slotValue) {
          var fnPr = particle.callFunction({
            deviceId: PARTICLE_DEVICE_ID,
            name: slot.name,
            argument: slotValue,
            auth: PARTICLE_ACCESS_TOKEN
          });

          fnPr.then(
            function(data) {
              console.log('Function called succesfully:', data);

              var speechOutput = "The light is now " + slotValue;
              response.tellWithCard(speechOutput, "Particle Light", speechOutput);
            }, function(err) {
              console.log('An error occurred:', err);
            });
        }
        else {
          response.tell("Sorry, I didn't catch what you said");
        }
    },
    "ParticleColorIntent": function (intent, session, response) {
        var slot = intent.slots.color;
        var slotValue = slot ? slot.value : "";

        if(slotValue) {
          var fnPr = particle.callFunction({
            deviceId: PARTICLE_DEVICE_ID,
            name: slot.name,
            argument: slotValue,
            auth: PARTICLE_ACCESS_TOKEN
          });

          fnPr.then(
            function(data) {
              console.log('Function called succesfully:', data);

              var speechOutput = "The light is now " + slotValue;
              response.tellWithCard(speechOutput, "Particle Light", speechOutput);
            }, function(err) {
              console.log('An error occurred:', err);
            });
        }
        else {
          response.tell("Sorry, I didn't catch what you said");
        }
    },
    "ParticleBrightnessIntent": function (intent, session, response) {
        var slot = intent.slots.brightness;
        var slotValue = slot ? slot.value : "";

        if(slotValue) {
          var fnPr = particle.callFunction({
            deviceId: PARTICLE_DEVICE_ID,
            name: slot.name,
            argument: slotValue,
            auth: PARTICLE_ACCESS_TOKEN
          });

          fnPr.then(
            function(data) {
              console.log('Function called succesfully:', data);
              var speechOutput = "";

              if(data.body.return_value > -1) {
                speechOutput = "The brightness level is now " + slotValue;
              }
              else {
                speechOutput = "The brightness level is invalid. Please choose a value between 0 and 255.";
              }

              response.tellWithCard(speechOutput, "Particle Light", speechOutput);
            }, function(err) {
              console.log('An error occurred:', err);
            });
        }
        else {
          response.tell("Sorry, I didn't catch what you said");
        }
    },
    "AMAZON.HelpIntent": function (intent, session, response) {
        var speechOutput = "You can ask me to turn on or off. You can also ask me to change colors, like red, green, blue, or white.";
        response.ask(speechOutput);
    }
};

// Create the handler that responds to the Alexa Request.
exports.handler = function (event, context) {
    // Create an instance of the ParticleSkill skill.
    var particleSkill = new ParticleSkill();
    particleSkill.execute(event, context);
};
