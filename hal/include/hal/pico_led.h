/**
 * @file pico_led.h
 * @brief Singleton class for controlling the onboard LED using GPIO.
 *
 * This class initializes the onboard LED GPIO and allows it to be toggled on/off.
 * It enforces a single active instance to avoid multiple configurations of the same pin.
 *
 * @author Hayden Mai
 * @date Apr-21-2025
 */

#ifndef PICO_LED_H_
#define PICO_LED_H_

/**
 * @class PicoLED
 * @brief Controls the onboard LED using GPIO.
 *
 * Simple interface for toggling a single LED on/off using a boolean flag.
 *
 * @note Class invariants:
 * - Only one instance of PicoLED can exist at a time.
 */
class PicoLED {
  public:
    /**
     * @brief Retrieves the one-and-only PicoLED instance.
     *
     * Initializes internal LED control if it hasn't already been done.
     *
     * @return Reference to the singleton PicoLED instance.
     * @post If not previously created, the instance is initialized.
     * @note Safe to call multiple times; always returns the same instance.
     */
    static PicoLED &get() noexcept;

    /**
     * @brief Turn the LED on or off.
     * @param on  true → LED on, false → LED off.
     * @post  LED GPIO line set to the requested level.
     */
    void set(bool on) noexcept;

    /** @brief Turns the LED on. */
    void on() noexcept;

    /** @brief Turns the LED off. */
    void off() noexcept;

    /// @cond DOXYGEN_HIDE
    // Delete copy/move so no second instance can be made
    PicoLED(const PicoLED &)            = delete; // Disables copy constructor
    PicoLED &operator=(const PicoLED &) = delete; // Disables copy-assignment operator
    PicoLED(PicoLED &&)                 = delete; // Disables move constructor
    PicoLED &operator=(PicoLED &&)      = delete; // Disables move-assigment opertor
                                                  /// @endcond

  private:
    PicoLED()  = default;
    ~PicoLED() = default;
};

#endif