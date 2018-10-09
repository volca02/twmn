#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QLabel>
#include <QQueue>
#include "settings.h"
#include "message.h"
#include "dbusinterface.h"

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(const char* wname, int screen = -1);
    ~Widget();

private slots:

    void                    init();
    void                    onDataReceived();
    void                    appendMessageToQueue(const Message& msg);
    void                    processMessageQueue();

    void                    updateTopLeftAnimation(QVariant value);
    void                    updateTopRightAnimation(QVariant value);
    void                    updateBottomRightAnimation(QVariant value);
    void                    updateBottomLeftAnimation(QVariant value);
    void                    updateTopCenterAnimation(QVariant value);
    void                    updateBottomCenterAnimation(QVariant value);
    void                    updateCenterAnimation(QVariant value);
    void                    startBounce();
    void                    unbounce();
    void                    doneBounce();
    void                    updateBounceAnimation(QVariant value);
    void                    reverseTrigger();
    void                    reverseStart();

    void                    updateFinalWidth();


public slots:
    // Called from the ShortcutGrabber
    void                    onPrevious();

    void                    onNext();

    /*!
      * \brief Run a command when the user activate the notification
      */
    void                    onActivate();

    /*!
      * \brief Hide the notification
      */
    void                    onHide();

    /*!
      * \brief Display the next notification as if the user invoqued onNext()
      */
    void                    autoNext();

    void                    mousePressEvent(QMouseEvent *);

    void                    wheelEvent(QWheelEvent *e);

    /*!
     * \brief processRemoteControl Executes a command received from the client
     * \param command the command to run [activate|hide|previous|next]
     */
    void                    processRemoteControl(QString command);

    /*!
     * \brief Receive signals from DBus. Each sent signal contains deserialized Message structure.
     * \param DBusInterface which signals receiving a new DBus message.
     */
    void                    connectToDBus(const DBusInterface& dbus);

private:
    /*!
      * \brief Get the final width of the slide after everything is set.
      */
    int                     computeWidth();

    void                    setupFont();

    void                    setupColors();

    /*!
      * \brief Create an appropriate connection according to the position parameter from Settings
      */
    void                    connectForPosition(QString position);

    /*!
      * \brief Set the icon.
      */
    void                    setupIcon();

    /*!
      * \brief Set the "title" widget content according to the front() Message.
      */
    void                    setupTitle();

    /*!
      * \brief Set the "text" widget content according to the front() Message.
      */
    void                    setupContent();

    /*!
      * \brief Load default settings for the front() Message according to the specified profile and configuration files.
      */
    void                    loadDefaults();

    /*!
      * \brief Tries to load a Pixmap from pattern : from a file, from a setting value.
      */
    QPixmap                 loadPixmap(QString pattern);

    /*!
      * \brief Update the message m if it's already in the queue
      * \return true if a message has been updated
      */
    bool                    update(const Message& m);

    QPoint                  stringToPos(QString string);

    inline std::size_t      getHeight();

private:
    Settings                m_settings;
    QUdpSocket              m_socket;
    QMap<QString, QLabel*>  m_contentView;
    QQueue<Message>         m_messageQueue;
    QParallelAnimationGroup m_animation;
    QTimer                  m_visible;
    QStack<Message>         m_previousStack;
    QPoint                  tmpBouncePos;
    int                     m_computedWidth;
    int                     m_screen;
    std::string             m_activePositionSlot;
};

#endif // WIDGET_H
