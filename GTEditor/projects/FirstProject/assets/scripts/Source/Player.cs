using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GT;
namespace FirstProject
{
    public class Player : Entity
    {
        public float Speed=1.0f;
        public float Time = 0.0f;
        void OnCreate(ulong uuid)
        {
            ID = uuid;
            Console.WriteLine($"Player.OnCreate - {ID}");
        }
        void OnUpdate(float ts)
        {
            //Console.WriteLine($"Player.OnUpdate: {ts}");
            Time += ts;
            float speed = Speed;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.Up))
                velocity.Y = Speed;
            else if (Input.IsKeyDown(KeyCode.Down))
                velocity.Y = -Speed;

            if (Input.IsKeyDown(KeyCode.Left))
                velocity.X = -Speed;
            else if (Input.IsKeyDown(KeyCode.Right))
                velocity.X = Speed;


            velocity *= speed * ts;


            Vector3 translation = Translation;
            translation += velocity;
            Translation = translation;
        }

    }
}