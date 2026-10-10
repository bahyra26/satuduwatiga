#pragma once
#include "MathUtils.h"
#include "Field.h"
#include "Goal.h"
#include "Ball.h"
#include "Sensor.h"

class Robot {
public:
    // kamera: segitiga di depan robot, kedalaman 3 petak (1.5 m), baris ke-1/2/3 lebarnya 3/5/7 petak
    // (alas 7 petak = 3.5 m), setara sudut pandang 90 derajat
    static constexpr double DEFAULT_FOV   = 90.0;
    static constexpr double DEFAULT_RANGE = 1.5;   // meter
    static constexpr double TURN_STEP     = 45.0;  // derajat per putaran

    // rencana tembakan: berdiri di standCell menghadap bola (heading), lalu bola ditendang ke angle
    // (lurus / miring atas / miring bawah relatif heading, jadi angle = heading + {0, +45, -45})
    struct ShotPlan {
        Cell standCell;
        double angle;    // sudut global tendangan (kelipatan 45)
        bool scores;     // hasil simulasi: bola berakhir di gawang?
        double heading;  // hadapan robot di standCell (menghadap bola)
    };

    // keputusan hasil think(): apa yang sebaiknya dilakukan robot sekarang
    enum class Action { Search, Align, Kick };

private:
    MathUtils::Vec2 pos_;  // meter, selalu di tengah petak
    double heading_;       // derajat, selalu kelipatan 45 (8 arah grid)
    Sensor sensor_;        // Robot HAS-A Sensor (composition)
    Goal goal_;            // robot SELALU tahu lokasi gawang lawan

    // satu langkah (1 petak) ke arah angleDeg (dibulatkan ke 45), hadap ikut arah itu
    bool stepAlong(double angleDeg, const Field& field);

    // simulasi tendangan ke angle pada salinan bola: berakhir di gawang?
    bool kickScores(const Ball& ball, const Field& field, double angle) const;

public:
    Robot(const MathUtils::Vec2& pos, double heading, const Field& field,
          const Goal& goal = Goal(), double fov = DEFAULT_FOV, double range = DEFAULT_RANGE);
    virtual ~Robot() = default;
    Robot(const Robot&) = default;
    Robot& operator=(const Robot&) = default;

    // keputusan dasar: bola di depan -> Kick, kelihatan -> Align, selain itu Search.
    // subclass (mis. Striker) meng-override dengan strategi sendiri
    virtual Action think(const Ball& ball, const Field& field) const;

    const Sensor& getSensor() const;
    double getFov() const;
    double getRange() const;
    void setVision(double fovDeg, double rangeM);
    bool canSee(const MathUtils::Vec2& target) const;
    double distanceTo(const MathUtils::Vec2& target) const;
    double relativeAngleTo(const MathUtils::Vec2& target) const;

    MathUtils::Vec2 getPosition() const;
    double getHeading() const;
    void setPosition(const MathUtils::Vec2& p, const Field& field);
    void setHeading(double deg);  // throw invalid_argument kalau bukan kelipatan 45

    const Goal& getGoal() const;
    void setGoal(const Goal& goal);

    Cell frontCell(const Field& field) const;  // petak tepat di depan robot

    double goalDistance() const;
    double goalBearing() const;        // sudut global
    double goalRelativeAngle() const;  // relatif ke hadapan robot

    // putar hadapan sebesar deg derajat (+ berlawanan arah jarum jam), harus kelipatan 45
    void rotate(double deg);

    // scanning: kalau bola belum kelihatan, putar stepDeg derajat
    // return true kalau bola kelihatan (setelah diputar)
    bool scanStep(const Ball& ball, double stepDeg = TURN_STEP);

    // putar satu lingkaran penuh sambil mencari bola
    // return true kalau bola kelihatan di salah satu arah
    bool scanFull(const Ball& ball, double stepDeg = TURN_STEP);

    // maju satu petak ke arah target (8 arah)
    // avoid (opsional): petak yang tidak boleh diinjak (mis. petak bola), robot memutar lewat samping
    // return false kalau sudah sampai atau tidak ada jalan
    bool stepToward(const MathUtils::Vec2& target, const Field& field, const Cell* avoid = nullptr);

    // ALIGN: coba 8 petak di sekitar bola x 3 arah tendangan (lurus / miring atas / miring bawah),
    // simulasikan ke mana bola berakhir, utamakan yang masuk gawang (kalau ada beberapa, yang
    // paling searah gawang, lalu yang paling dekat untuk dijalani)
    ShotPlan planShot(const Ball& ball, const Field& field) const;

    // satu aksi menuju posisi tendang: jalan (menghindari petak bola), lalu putar menghadap bola.
    // return false kalau sudah siap tendang atau tidak ada jalan
    bool alignToShoot(const Ball& ball, const Field& field);

    // syarat tendang: bola tepat di petak depan robot
    bool canKick(const Ball& ball, const Field& field) const;

    // tendang bola, hanya kalau canKick().
    // arah tendangan: lurus / miring atas / miring bawah relatif hadapan robot (yang masuk gawang,
    // kalau ada beberapa yang paling searah gawang). return true kalau berhasil menendang
    bool kickBall(Ball& ball, const Field& field);
};