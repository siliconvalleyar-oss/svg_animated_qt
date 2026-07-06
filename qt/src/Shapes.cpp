#include "Shapes.h"

static const QString NS = QStringLiteral("http://www.w3.org/2000/svg");

static const QVector<Shapes::Shape> SHAPES = {
    { QStringLiteral("Circulo"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><circle cx=\"100\" cy=\"100\" r=\"70\" fill=\"none\" stroke=\"#6c5ce7\" stroke-width=\"3\"/></svg>").arg(NS) },
    { QStringLiteral("Cuadrado"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><rect x=\"30\" y=\"30\" width=\"140\" height=\"140\" rx=\"8\" fill=\"none\" stroke=\"#e74c3c\" stroke-width=\"3\"/></svg>").arg(NS) },
    { QStringLiteral("Triangulo"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><polygon points=\"100,20 180,170 20,170\" fill=\"none\" stroke=\"#2ecc71\" stroke-width=\"3\" stroke-linejoin=\"round\"/></svg>").arg(NS) },
    { QStringLiteral("Estrella"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><polygon points=\"100,15 125,75 190,80 140,125 155,190 100,155 45,190 60,125 10,80 75,75\" fill=\"none\" stroke=\"#f39c12\" stroke-width=\"3\" stroke-linejoin=\"round\"/></svg>").arg(NS) },
    { QStringLiteral("Corazon"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><path d=\"M100 170 C60 130 20 100 20 65 C20 35 45 15 70 15 C85 15 95 25 100 35 C105 25 115 15 130 15 C155 15 180 35 180 65 C180 100 140 130 100 170Z\" fill=\"none\" stroke=\"#e74c3c\" stroke-width=\"3\"/></svg>").arg(NS) },
    { QStringLiteral("Hexagono"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><polygon points=\"100,15 175,50 175,140 100,180 25,140 25,50\" fill=\"none\" stroke=\"#1abc9c\" stroke-width=\"3\" stroke-linejoin=\"round\"/></svg>").arg(NS) },
    { QStringLiteral("Rombo"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><polygon points=\"100,15 185,100 100,185 15,100\" fill=\"none\" stroke=\"#9b59b6\" stroke-width=\"3\" stroke-linejoin=\"round\"/></svg>").arg(NS) },
    { QStringLiteral("Cruz"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><path d=\"M70 30 H130 V70 H170 V130 H130 V170 H70 V130 H30 V70 H70 Z\" fill=\"none\" stroke=\"#3498db\" stroke-width=\"3\" stroke-linejoin=\"round\"/></svg>").arg(NS) },
    { QStringLiteral("Onda"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><path d=\"M20 100 Q50 60 80 100 Q110 140 140 100 Q170 60 200 100\" fill=\"none\" stroke=\"#e67e22\" stroke-width=\"3\" stroke-linecap=\"round\"/></svg>").arg(NS) },
    { QStringLiteral("Flecha"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><path d=\"M100 30 L170 100 L130 100 L130 170 L70 170 L70 100 L30 100 Z\" fill=\"none\" stroke=\"#6c5ce7\" stroke-width=\"3\" stroke-linejoin=\"round\"/></svg>").arg(NS) },
    { QStringLiteral("Rayo"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><polygon points=\"115,15 50,105 90,105 80,185 155,90 110,90\" fill=\"none\" stroke=\"#f1c40f\" stroke-width=\"3\" stroke-linejoin=\"round\"/></svg>").arg(NS) },
    { QStringLiteral("Luna"),
      QStringLiteral("<svg xmlns=\"%1\" viewBox=\"0 0 200 200\"><path d=\"M120 30 A65 65 0 1 0 120 170 A50 50 0 1 1 120 30\" fill=\"none\" stroke=\"#8e44ad\" stroke-width=\"3\"/></svg>").arg(NS) },
};

const QVector<Shapes::Shape> &Shapes::all() {
    return SHAPES;
}

QString Shapes::svgByName(const QString &name) {
    for (const auto &s : SHAPES) {
        if (s.name == name) return s.svg;
    }
    return {};
}
