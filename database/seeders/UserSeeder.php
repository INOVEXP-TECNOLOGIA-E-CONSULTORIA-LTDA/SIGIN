<?php

namespace Database\Seeders;

use App\Models\User;
use Illuminate\Database\Seeder;
use Illuminate\Support\Facades\Hash;
use Spatie\Permission\Models\Role;

class UserSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        $user1 = User::create([
            'name' => 'João Pedro Ghinozzi',
            'email' => 'ghinozzi@gmail.com',
            'password' => Hash::make('123'),
            'habilitado' => true
        ]);

        $user2 = User::create([
            'name' => 'Usuário Teste Admin',
            'email' => 'admin@filamentphp.com',
            'password' => Hash::make('password'),
            'habilitado' => true
        ]);

        $adminRole = Role::query()->where('name', 'super-admin')->first();
        $user1->assignRole($adminRole);
        $user2->assignRole($adminRole);
    }
}
