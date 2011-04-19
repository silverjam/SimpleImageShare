#include <QtCore/QString>
#include <QtTest/QtTest>

#include "sislibrary.h"

using namespace CommandInformation;

class SISLibraryTest : public QObject
{
    Q_OBJECT

public:
    SISLibraryTest();

private Q_SLOTS:
    void testProtoVersion();
    void testDiscoverImageSets();
};

SISLibraryTest::SISLibraryTest()
{
}

class TestCommanSink : public ICommandSink
{
public:
    TestCommanSink()
        : m_b_handle_DiscoverImageSets(false)
        , m_b_handle_ProtocolVersion(false)
    {}

    DiscoverImageSets m_DiscoverImageSets;
    bool m_b_handle_DiscoverImageSets;
    void handle_DiscoverImageSets(const DiscoverImageSets& input)
    {
        m_b_handle_DiscoverImageSets = true;
        m_DiscoverImageSets = input;
    }

    ProtocolVersion m_ProtocolVersion;
    bool m_b_handle_ProtocolVersion;
    void handle_ProtocolVersion(const ProtocolVersion& input)
    {
        m_b_handle_ProtocolVersion = true;
        m_ProtocolVersion = input;
    }

    bool nothingCalled()
    {
        return
            ! m_b_handle_DiscoverImageSets &&
            ! m_b_handle_ProtocolVersion
        ;
    }
};

void SISLibraryTest::testProtoVersion()
{
    QBuffer buf;
    buf.open(QIODevice::ReadWrite);

    TestCommanSink sink;

    QDataStream ds1(&buf);

    SisCommands cmds1(ds1, &sink);
    cmds1.build_ProtocolVersion();

    QVERIFY( sink.nothingCalled() );

    buf.seek(0);
    QDataStream ds2(&buf);

    SisCommands cmds2(ds2, &sink);
    QVERIFY( cmds2.parseOne() );

    QVERIFY( sink.m_b_handle_ProtocolVersion );
    QVERIFY( ! sink.nothingCalled() );
}

void SISLibraryTest::testDiscoverImageSets()
{
    QBuffer buf;
    buf.open(QIODevice::ReadWrite);

    TestCommanSink sink;

    QDataStream ds1(&buf);

    SisCommands cmds1(ds1, &sink);
    cmds1.build_DiscoverImageSets(42);

    QVERIFY( sink.nothingCalled() );

    buf.seek(0);
    QDataStream ds2(&buf);

    SisCommands cmds2(ds2, &sink);
    QVERIFY( cmds2.parseOne() );

    QVERIFY( sink.m_b_handle_DiscoverImageSets );
    QVERIFY( ! sink.nothingCalled() );

    QVERIFY( sink.m_DiscoverImageSets.count == 42 );
}

QTEST_APPLESS_MAIN(SISLibraryTest);

#include "tst_sislibrarytest.moc"
