using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace proyecto2.Models
{
    public class Ahorro
    {
        private int fecha, ahorro_agua;

        public Ahorro(int fecha, int ahorro_agua)
        {
            this.Fecha = fecha;
            this.Ahorro_agua = ahorro_agua;
        }

        public int Fecha { get => fecha; set => fecha = value; }
        public int Ahorro_agua { get => ahorro_agua; set => ahorro_agua = value; }
    }
}
