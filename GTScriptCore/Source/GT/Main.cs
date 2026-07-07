using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GT
{
    public class Main : Entity
    {
        void OnCreate(ulong uuid)
        {
            ID = uuid;
            Console.WriteLine($"Player.OnCreate - {ID}");
        }
        void OnUpdate(float ts)
        {
             Console.WriteLine($"Player.OnUpdate: {ts}");

            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.Up))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.Down))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.Left))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.Right))
                velocity.X = 1.0f;


            velocity *= speed * ts;


            Vector3 translation = Translation;
            translation += velocity;
            Translation = translation;
        }
    }
}