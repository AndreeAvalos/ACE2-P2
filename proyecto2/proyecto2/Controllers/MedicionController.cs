using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using proyecto2.Models;

namespace proyecto2.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class MedicionController : ControllerBase
    {
        readonly string cadenaconexion = "Server=tcp:ws1.database.windows.net,1433;Initial Catalog=Arqui2-P2;Persist Security Info=False;User ID=ANDREE;Password=1234kINGAVALOS;MultipleActiveResultSets=False;Encrypt=True;TrustServerCertificate=False;Connection Timeout=30;";

        // GET: api/Medicion
        [HttpGet]
        public string Get()
        {
            //DateTime currentTime = DateTime.Now;
            //string fecha_actual = (TimeZoneInfo.ConvertTimeBySystemTimeZoneId(currentTime, TimeZoneInfo.Local.Id, "Central Standard Time")).ToString();
            //Medicion nuevo = new Medicion(1, 1, fecha_actual, "Rock", 200);

            //return JsonConvert.SerializeObject(nuevo);
            //return fecha_actual;


            SqlDataReader reader = null;
            SqlConnection conexion = new SqlConnection
            {
                ConnectionString = cadenaconexion
            };

            SqlCommand cmd = new SqlCommand
            {
                CommandType = CommandType.Text,
                CommandText = "SELECT * FROM Ducha",
                Connection = conexion
            };
            conexion.Open();

            reader = cmd.ExecuteReader();
            List<Medicion> lstMediciones = new List<Medicion>();
            string json;
            try
            {
                while (reader.Read())
                {
                    Medicion nuevo = new Medicion(
                         Convert.ToInt32(reader.GetValue(0)),
                         Convert.ToInt32(reader.GetValue(1)),
                         reader.GetValue(2).ToString(),
                         reader.GetValue(3).ToString(),
                         Convert.ToInt32(reader.GetValue(4))
                     );
                    lstMediciones.Add(nuevo);
                }
                json = JsonConvert.SerializeObject(lstMediciones);
            }
            catch (Exception)
            {
                json = "No existen datos";
            }

            conexion.Close();
            return json;




        }

        // GET: api/Medicion/5
        [HttpGet("{id}", Name = "Get")]
        public string Get(int id)
        {
            SqlDataReader reader = null;
            SqlConnection conexion = new SqlConnection
            {
                ConnectionString = cadenaconexion
            };

            string consulta = "";

            if (id == 3)
                consulta = "select DatePart(\"yyyy\",fecha) as Dia, sum(ahorro) as Agua_ahorrada from Ducha group by DatePart(\"yyyy\",fecha)";
            else if (id == 2)
                consulta = "select DatePart(\"m\",fecha) as Dia, sum(ahorro) as Agua_ahorrada from Ducha group by DatePart(\"m\",fecha)";
            else
                consulta = "select DatePart(\"d\",fecha) as Dia, sum(ahorro) as Agua_ahorrada from Ducha group by DatePart(\"d\",fecha)";

            SqlCommand cmd = new SqlCommand
            {
                CommandType = CommandType.Text,
                CommandText = consulta,
                Connection = conexion
            };
            conexion.Open();

            reader = cmd.ExecuteReader();
            List<Ahorro> lst_ahorro = new List<Ahorro>();
            string json = "";

            try
            {
                while (reader.Read())
                {
                    Ahorro nuevo = new Ahorro(
                    Convert.ToInt32(reader.GetValue(0).ToString()),
                    Convert.ToInt32(reader.GetValue(1).ToString())
                     );
                    lst_ahorro.Add(nuevo);
                }
                json = JsonConvert.SerializeObject(lst_ahorro);
            }
            catch (Exception)
            {
                json = "No existen datos";
            }

            conexion.Close();
            return json;
        }

        // POST: api/Medicion
        [HttpPost]
        public bool Post(Medicion entrada)
        {
            bool salida = false;
            DateTime currentTime = DateTime.Now;
            string fecha_actual = (TimeZoneInfo.ConvertTimeBySystemTimeZoneId(currentTime, TimeZoneInfo.Local.Id, "Central Standard Time")).ToString();

            SqlConnection conexion = new SqlConnection
            {
                ConnectionString = cadenaconexion
            };
            conexion.Open();
            try
            {


                SqlCommand cmd = new SqlCommand("insertar_Medicion", conexion)
                {
                    CommandType = CommandType.StoredProcedure
                };

                cmd.Parameters.AddWithValue("tiempo", entrada.Tiempo);
                cmd.Parameters.AddWithValue("fecha", fecha_actual);
                cmd.Parameters.AddWithValue("genero", entrada.Genero);
                cmd.Parameters.AddWithValue("ahorro", entrada.Ahorro);

                //Medicion nue = new Medicion(1, nuevo.Tipo, nuevo.Carnet, nuevo.Valor_medicion, fecha_actual, hora, nuevo.Latitud, nuevo.Longitud, nuevo.Unidad);
                cmd.ExecuteNonQuery();
                salida = true;

            }
            catch (Exception)
            {
            }
            conexion.Close();


            return salida;
        }

        // PUT: api/Medicion/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody] string value)
        {
        }

        // DELETE: api/ApiWithActions/5
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
        }
    }
}
