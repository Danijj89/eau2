<?php

    $file = fopen($argv[1], 'w');
    $num_lines = (int)$argv[2];
    $field_in_row = (int)$argv[3];
    $max_rand_num = (int)$argv[4];

    for ($i = 0; $i < $num_lines; $i++) {
        $line = '';
        for ($j = 0; $j < $field_in_row; $j++) {
            $line .= '<' . mt_rand(1, $max_rand_num) . '>';
        }
        $line .= "\n";
        fwrite($file, $line);
    }

    fclose($file);
