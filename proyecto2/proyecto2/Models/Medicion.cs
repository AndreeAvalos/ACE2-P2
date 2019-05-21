using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace proyecto2.Models
{
    public class Medicion
    {
        private int id_ducha, tiempo;
        private string fecha, genero;
        private int ahorro;

        public Medicion(int id_ducha, int tiempo, string fecha, string genero, int ahorro)
        {
            this.Id_ducha = id_ducha;
            this.Tiempo = tiempo;
            this.Fecha = fecha;
            this.Genero = genero;
            this.Ahorro = ahorro;
        }

        public int Id_ducha { get => id_ducha; set => id_ducha = value; }
        public int Tiempo { get => tiempo; set => tiempo = value; }
        public string Fecha { get => fecha; set => fecha = value; }
        public string Genero { get => genero; set => genero = value; }
        public int Ahorro { get => ahorro; set => ahorro = value; }
    }
}
