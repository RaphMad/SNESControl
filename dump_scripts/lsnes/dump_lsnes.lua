dumpfile = nil;
latchCount = 0;

start_dump = function(filename)
    fh, err = io.open(filename .. ".bin", "wb");
    if not fh then
        error("Error opening output file: " .. err);
    end
    print(string.format('Dumping to %s.bin', filename));

    dumpfile = fh;
    latchCount = 0;
end

end_dump = function()
    if (dumpfile) then
        dumpfile:close();
        dumpfile = nil;
        print("Dumping halted")
    else
        print("Cannot stop: Dumping not started")
    end
end

-- called when latch line is RISING
on_latch = function()
    if dumpfile then
        latchCount = latchCount + 1;
        currentFrame = movie.get_frame(movie.find_frame(movie.currentframe()));

        byte1 = 0xFF
        byte2 = 0xFF

        for buttonIndex = 0, 7 do
            if currentFrame:get_button(1, 0, buttonIndex) then
                byte1 = bit.band(byte1, ~bit.lshift(1, buttonIndex))
            end

            if currentFrame:get_button(1, 0, buttonIndex + 8) then
                byte2 = bit.band(byte2, ~bit.lshift(1, buttonIndex))
            end
        end

        dumpfile:write(string.char(byte1));
        dumpfile:write(string.char(byte2));
    end
end

on_paint = function()
    if dumpfile then
        gui.text(0, 0, latchCount, 0x00FF00, 0);
    end
end
