# How to contribute

To contribute to the AtHome framework, you can help to the following things:

## Development

You can help to the development of the framework by proposing some new features and solving issues.

### Practices

- Fork the master branch
- Commit your work frequently
- When your development is ready, pull the updates in the master branch and merge modifications into your branch
- Push your branch to the remote repository
- Ensures your modifications pass correctly through all the tests
- Do a merge request

### Issues

Feel free to look at open issues and propose your fixes.

### Examples

If you develop new modules and want to share them, feel free to propose them as example.

### Tests

To always ensure a better quality of the code of this project, there's never enough tests.
So if you have some ideas, feel free to develop new tests of any kind.

For now the pipeline of this project is able to simulate an AVR running some code produced with this framework
and ArduinoUnit is used for unit tests.

### Components

To add new components to the framework, just create a new class inheriting from abstract classes of the framework.

#### Sensors

Currently, here is the list of supported types of sensors by the framework and their base classes:

- Temperature sensor: `athome::sensor::ATemperatureSensor`
- Humidity sensor: `athome::sensor::AHumiditySensor`
- Luminosity sensor: `athome::sensor::ALuminositySensor`

#### RGB Displays

AtHome modules display information by a simple uniform colour code, to implement a new display compatible with the framework,
just inherit the `athome::display::ARGBLed` class

#### Storage

AtHome modules store persistent data by using components implementing the `athome::IStorage` interface.
If you want to add a new kind of persistent storage to the framework you just have to implement this interface.

## Documentation

To help new peoples interested into the project, feel free to write as much as you want documentation and tutorial and translating
existing documentation into other languages.